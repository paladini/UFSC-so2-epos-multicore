// EPOS Alarm Abstraction Declarations

#ifndef __alarm_h
#define __alarm_h

#include <utility/queue.h>
#include <utility/handler.h>
#include <tsc.h>
#include <rtc.h>
#include <ic.h>
#include <timer.h>
#include <semaphore.h>

__BEGIN_SYS

class Alarm
{
    friend class System;
    friend class Scheduling_Criteria::FCFS;

private:
    typedef TSC::Hertz Hertz;
    typedef Timer::Tick Tick;  

    typedef Relative_Queue<Alarm, Tick> Queue;

public:
    typedef RTC::Microsecond Microsecond;
    
    // Infinite times (for alarms)
    enum { INFINITE = RTC::INFINITE };
    
public:
    Alarm(const Microsecond & time, Handler * handler, int times = 1);
    ~Alarm();

    static Hertz frequency() { return _timer->frequency(); }

    static void delay(const Microsecond & time);

private:
    static void init();

    static Microsecond period() {
        return 1000000 / frequency();
    }

    static Tick ticks(const Microsecond & time) {
        return (time + period() / 2) / period();
    }

    static void lock() { Thread::lock(); }
    static void unlock() { Thread::unlock(); }

    static void handler(const IC::Interrupt_Id & i);

private:
    Tick _ticks;
    Handler * _handler;
    int _times; 
    Queue::Element _link;

    static Alarm_Timer * _timer;
    static volatile Tick _elapsed;
    static Queue _request;
};


class Delay
{
private:
    typedef RTC::Microsecond Microsecond;

public:
    Delay(const Microsecond & time): _time(time)  { Alarm::delay(_time); }

private:
    Microsecond _time;
};


// The following Scheduling Criteria depend on Alarm, which is not yet available at scheduler.h
namespace Scheduling_Criteria {
    inline FCFS::FCFS(int p): Priority((p == IDLE) ? IDLE : Alarm::_elapsed) {}
};

__END_SYS

#endif
