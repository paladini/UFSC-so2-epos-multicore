// EPOS System Abstraction Initialization

#include <system.h>
#include <alarm.h>

__BEGIN_SYS

void System::init()
{
    if(Traits<Alarm>::enabled)
        Alarm::init();

    if(Traits<Thread>::enabled)
        Thread::init();
}

__END_SYS
