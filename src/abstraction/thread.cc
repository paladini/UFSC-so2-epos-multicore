// EPOS Thread Abstraction Implementation

#include <machine.h>
#include <thread.h>
#include <system.h>

// This_Thread class attributes
__BEGIN_UTIL
bool This_Thread::_not_booting;
__END_UTIL

__BEGIN_SYS

// Class attributes
volatile unsigned int Thread::_thread_count;
Scheduler_Timer * Thread::_timer;

Scheduler<Thread> Thread::scheduler;
// Methods
void Thread::constructor_prolog(unsigned int stack_size)
{
    lock();

    _thread_count++;

    _stack = new (SYSTEM) char[stack_size];
}


void Thread::constructor_epilog(const Log_Addr & entry, unsigned int stack_size)
{
    db<Thread>(TRC) << "Thread(entry=" << entry
                    << ",state=" << _state
                    << ",priority=" << _link.rank()
                    << ",stack={b=" << reinterpret_cast<void *>(_stack)
                    << ",s=" << stack_size
                    << "},context={b=" << _context
                    << "," << *_context << "}) => " << this << endl;

    if(_state == State::READY || _state == State::RUNNING){
    	scheduler.insert(&_link);
    }

    if(preemptive && (_state == READY) && (_link.rank() != Criterion::IDLE))
        reschedule();
    else
        unlock();
}


Thread::~Thread()
{
    lock();

    db<Thread>(TRC) << "~Thread(this=" << this
                    << ",state=" << _state
                    << ",priority=" << _link.rank()
                    << ",stack={b=" << reinterpret_cast<void *>(_stack)
                    << ",context={b=" << _context
                    << "," << *_context << "})" << endl;

    // The running thread cannot delete itself!
    assert(_state != RUNNING);

    switch(_state) {
    case RUNNING:  // For switch completion only: the running thread would have deleted itself! Stack wouldn't have been released!
        exit(-1);
        break;
    case READY:
        scheduler.remove(&_link);
        _thread_count--;
        break;
    case SUSPENDED:
        _thread_count--;
        break;
    case WAITING:
        _waiting->remove(this);
        _thread_count--;
        break;
    case FINISHING: // Already called exit()
        break;
    }

    if(_joining)
        _joining->resume();

    unlock();

    delete _stack;
}


int Thread::join()
{
    lock();

    db<Thread>(TRC) << "Thread::join(this=" << this << ",state=" << _state << ")" << endl;

    // Precondition: no Thread::self()->join()
    assert(running() != this);

    // Precondition: a single joiner
    assert(!_joining);

    if(_state != FINISHING) {
        _joining = running();
        _joining->suspend();
    } else
        unlock();

    return *reinterpret_cast<int *>(_stack);
}


void Thread::pass()
{
    lock();

    db<Thread>(TRC) << "Thread::pass(this=" << this << ")" << endl;

    Thread * prev = running();
    prev->_state = READY;

    scheduler.choose(&_link);
    _state = RUNNING;

    dispatch(prev, this);

    unlock();
}


void Thread::suspend()
{
    lock();

    db<Thread>(TRC) << "Thread::suspend(this=" << this << ")" << endl;

    scheduler.remove(&_link);
    _state = SUSPENDED;

    Thread* next = running();
    next->_state = RUNNING;

    dispatch(this, next);

    unlock();
}


void Thread::resume()
{
    lock();

    db<Thread>(TRC) << "Thread::resume(this=" << this << ")" << endl;

   _state = READY;
   scheduler.insert(&_link);

   unlock();

}


// Class methods
void Thread::yield()
{
    lock();

    db<Thread>(TRC) << "Thread::yield(running=" << running() << ")" << endl;

    Thread * prev = running();
    prev->_state = READY;
    Thread* next = scheduler.choose_another()->object();
    next->_state = RUNNING;

    dispatch(prev, next);

    unlock();
}


void Thread::exit(int status)
{
    lock();

    db<Thread>(TRC) << "Thread::exit(status=" << status << ") [running=" << running() << "]" << endl;

    Thread * prev = running();
    scheduler.remove(&prev->_link);
    prev->_state = FINISHING;
    *reinterpret_cast<int *>(prev->_stack) = status;

    _thread_count--;

    if(prev->_joining) {
        prev->_joining->resume();
        prev->_joining = 0;
    }

    lock();

    Thread* next = running();
    next->_state = RUNNING;
    dispatch(prev, next);

    unlock();
}

void Thread::sleep(Queue * q)
{
    db<Thread>(TRC) << "Thread::sleep(running=" << running() << ",q=" << q << ")" << endl;

    // lock() must be called before entering this method
    assert(locked());

    Thread * prev = running();
    scheduler.remove(&prev->_link);

    prev->_state = WAITING;
    prev->_waiting = q;
    q->insert(&prev->_link);

    Thread* next = running();
    next->_state = RUNNING;

    dispatch(prev, next);

    unlock();
}


void Thread::wakeup(Queue * q)
{
    db<Thread>(TRC) << "Thread::wakeup(running=" << running() << ",q=" << q << ")" << endl;

    // lock() must be called before entering this method
    assert(locked());

    if(!q->empty()) {
        Thread * t = q->remove()->object();
        t->_state = READY;
        t->_waiting = 0;
        scheduler.insert(&t->_link);

        if(preemptive)
		   reschedule();
    }

    unlock();
}


void Thread::wakeup_all(Queue * q)
{
    db<Thread>(TRC) << "Thread::wakeup_all(running=" << running() << ",q=" << q << ")" << endl;

    // lock() must be called before entering this method
    assert(locked());

    while(!q->empty()) {
        Thread * t = q->remove()->object();
        t->_state = READY;
        t->_waiting = 0;
        scheduler.insert(&t->_link);
    }

    unlock();

    if(preemptive)
        reschedule();
}


void Thread::reschedule()
{
	lock();

    Thread * prev = running();
    Thread * next = scheduler.choose()->object();

    dispatch(prev, next);

    unlock();
}


void Thread::time_slicer(const IC::Interrupt_Id & i)
{
    reschedule();
}


void Thread::dispatch(Thread * prev, Thread * next)
{
    if(prev != next) {
        if(prev->_state == RUNNING)
            prev->_state = READY;
        next->_state = RUNNING;

        db<Thread>(TRC) << "Thread::dispatch(prev=" << prev << ",next=" << next << ")" << endl;
        db<Thread>(INF) << "prev={" << prev << ",ctx=" << *prev->_context << "}" << endl;
        db<Thread>(INF) << "next={" << next << ",ctx=" << *next->_context << "}" << endl;

        CPU::switch_context(&prev->_context, next->_context);
    }

    unlock();
}


int Thread::idle()
{
    while(true) {
        if(Traits<Thread>::trace_idle)
            db<Thread>(TRC) << "Thread::idle(this=" << running() << ")" << endl;

        if(_thread_count <= 1) { // Only idle is left
            CPU::int_disable();
            db<Thread>(WRN) << "The last thread has exited!" << endl;
            if(reboot) {
                db<Thread>(WRN) << "Rebooting the machine ..." << endl;
                Machine::reboot();
            } else {
                db<Thread>(WRN) << "Halting the machine ..." << endl;
                CPU::halt();
            }
        } else {
            CPU::int_enable();
            CPU::halt();
        }
    }

    return 0;
}

__END_SYS

// Id forwarder to the spin lock
__BEGIN_UTIL
unsigned int This_Thread::id()
{
    return _not_booting ? reinterpret_cast<volatile unsigned int>(Thread::self()) : Machine::cpu_id() + 1;
}
__END_UTIL
