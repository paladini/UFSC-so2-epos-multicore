// EPOS Thread Abstraction Initialization

#include <system.h>
#include <thread.h>
#include <alarm.h>

__BEGIN_SYS

void Thread::init()
{
    // The installation of the scheduler timer handler must precede the
    // creation of threads, since the constructor can induce a reschedule
    // and this in turn can call timer->reset()
    // Letting reschedule() happen during thread creation is harmless, since
    // MAIN is created first and dispatch won't replace it nor by itself
    // neither by IDLE (which has a lower priority)
    if(Criterion::timed)
        _timer = new (SYSTEM) Scheduler_Timer(QUANTUM, time_slicer);

    _rebalancer_timer = new (SYSTEM) Rebalancer_Timer(REBALANCER_QUANTUM, rebalance_handler);

    IC::int_vector(IC::INT_RESCHEDULER, reschedule_handler);
    IC::enable(IC::INT_RESCHEDULER);

    IC::int_vector(IC::INT_SUSPEND, suspend_handler);
    IC::enable(IC::INT_SUSPEND);
}

__END_SYS
