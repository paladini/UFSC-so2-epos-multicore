// EPOS Alarm Abstraction Implementation

#include <alarm.h>
#include <display.h>
#include <thread.h>
#include <system.h>
#include <utility/list.h>

__BEGIN_SYS

// Class attributes
Alarm_Timer * Alarm::_timer;
volatile Alarm::Tick Alarm::_elapsed;
Alarm::Queue Alarm::_request;


// Methods
Alarm::Alarm(const Microsecond & time, Handler * handler, int times)
: _ticks(ticks(time)), _handler(handler), _times(times), _link(this, _ticks)
{
    lock();

    db<Alarm>(TRC) << "Alarm(t=" << time << ",tk=" << _ticks << ",h=" << reinterpret_cast<void *>(handler)
                   << ",x=" << times << ") => " << this << endl;

    if(_ticks) {
        _request.insert(&_link);
        unlock();
    } else {
        unlock();
        (*handler)();
    }
}


Alarm::~Alarm()
{
    lock();

    db<Alarm>(TRC) << "~Alarm(this=" << this << ")" << endl;

    _request.remove(this);

    unlock();
}


class SuspendHandler : public Handler
{
public:
    SuspendHandler(Thread * thread) : _thread(thread) {}
    void operator()() { _thread->resume(); }

private:
    Thread * _thread;
};


// Class methods
void Alarm::delay(const Microsecond & time)
{
    db<Alarm>(TRC) << "Alarm::delay(time=" << time << ")" << endl;

    Thread * self = Thread::self();
    SuspendHandler sh(self);
    Alarm alarm(time, &sh, 1);
    self->suspend();
}


void Alarm::handler(const IC::Interrupt_Id & i)
{
    typedef Simple_List<Handler> HList;

    lock();

    ++_elapsed;

    if(Traits<Alarm>::visible) {
        Display display;
        int lin, col;
        display.position(&lin, &col);
        display.position(0, 79);
        display.putc(_elapsed);
        display.position(lin, col);
    }

    HList handler;

    if(!_request.empty()) {
        _request.head()->promote();
        while(_request.head()->rank() <= 0) {
            Queue::Element* e = _request.remove();
            Alarm* alarm = e->object();

            handler.insert(new (KERNEL) HList::Element(alarm->_handler));
            
            alarm->_times--;
            if(alarm->_times > 0) {
                e->rank(alarm->_ticks);
                _request.insert(e);
            }
        }
    }

    unlock();

    for (HList::Iterator it = handler.begin(); it != handler.end(); ++it) {
        Handler * h = it->object();
        (*h)();
    }
}

__END_SYS
