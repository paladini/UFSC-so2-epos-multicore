// EPOS Thread Abstraction Implementation

#include <machine.h>
#include <mutex.h>
#include <thread.h>
#include <system.h>

// This_Thread class attributes
__BEGIN_UTIL
bool This_Thread::_not_booting;
__END_UTIL

__BEGIN_SYS

// Class attributes
Scheduler_Timer * Thread::_timer;

Thread* volatile Thread::_running;
Thread::Queue Thread::_ready;
Thread::Queue Thread::_suspended;
unsigned int Thread::_active_count = -1;

// Methods
void Thread::constructor_prolog(unsigned int stack_size)
{
    lock();

    ++_active_count;
    _stack = reinterpret_cast<char *>(KERNEL);
    _joined = new (KERNEL) Mutex();
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

    switch(_state) {
        case RUNNING: break;
        case SUSPENDED: _suspended.insert(&_link); break;
        default: _ready.insert(&_link);
    }

    _joined->lock();

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

    _ready.remove(this);
    _suspended.remove(this);

    delete _joined;
    --_active_count;

    unlock();

    delete _stack;
}

int Thread::join()
{
    lock();

    db<Thread>(TRC) << "Thread::join(this=" << this << ",state=" << _state << ")" << endl;

    assert(_running != this);
    if(_state != FINISHING) {
        _joined->lock();
        _joined->unlock();
    }

    unlock();

    return *reinterpret_cast<int *>(_stack);
}


void Thread::pass()
{
    lock();

    db<Thread>(TRC) << "Thread::pass(this=" << this << ")" << endl;

    Thread * prev = _running;
    prev->_state = READY;
    _ready.insert(&prev->_link);

    _ready.remove(this);
    _state = RUNNING;
    _running = this;

    dispatch(prev, this);

    unlock();
}


void Thread::suspend()
{
    lock();

    db<Thread>(TRC) << "Thread::suspend(this=" << this << ")" << endl;

    if(_running != this)
        _ready.remove(this);

    Thread * _current = _running;
    _suspended.insert(&_link);
    _state = SUSPENDED;

    _running = _ready.remove()->object();
    _running->_state = RUNNING;

    dispatch(_current, _running);

    unlock();
}


void Thread::resume()
{
    lock();

    db<Thread>(TRC) << "Thread::resume(this=" << this << ")" << endl;

   _suspended.remove(this);
   _state = READY;
   _ready.insert(&_link);

   unlock();
}


void Thread::sleep(Thread::Queue& queue)
{
    Thread* t = _running->_link.object();
    Queue::Element* el = new (KERNEL) Queue::Element(t);
    queue.insert(el);
    _running->suspend();
}


void Thread::wakeup(Thread::Queue& queue)
{
    if (Thread::Queue::Element * suspended = queue.remove()) {
        suspended->object()->resume();
    }
    Thread::reschedule();
}


void Thread::wakeup_all(Thread::Queue& queue)
{
    for (unsigned int i = 0; i < queue.size(); ++i) {
        queue.remove()->object()->resume();
    }
    Thread::reschedule();
}


// Class methods
void Thread::yield()
{
    lock();

    db<Thread>(TRC) << "Thread::yield(running=" << _running << ")" << endl;

    Thread * prev = _running;
    prev->_state = READY;
    _ready.insert(&prev->_link);

    _running = _ready.remove()->object();
    _running->_state = RUNNING;

    dispatch(prev, _running);
}


void Thread::exit(int status)
{
    lock();

    db<Thread>(TRC) << "Thread::exit(status=" << status << ") [running=" << running() << "]" << endl;

    Thread * prev = _running;
    prev->_state = FINISHING;
    *reinterpret_cast<int *>(prev->_stack) = status;

    prev->_joined->unlock();
    _running = _ready.remove()->object();
    dispatch(prev, _running);

    unlock();
}


void Thread::reschedule()
{
    yield();
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
    db<Thread>(TRC) << "Thread::idle()" << endl;

    while (_active_count > 1) { // if there is only the idle/main thread
        CPU::int_enable();
        CPU::halt();

        if (_ready.size() > 1) { // idle thread is always ready
            yield(); // force rescheduling
        }
    }

    db<Thread>(INF) << "There are no runnable threads at the moment!" << endl;
    CPU::int_disable();

    if (reboot) {
        db<Thread>(INF) << "Rebooting the CPU ..." << endl;
        Machine::reboot();
    } else {
        db<Thread>(INF) << "Halting the CPU ..." << endl;
    }
    CPU::halt(); // halting without interruption = tango down

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
