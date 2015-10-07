#ifndef __priority_h
#define __priority_h

__BEGIN_SYS

class Priority
{
public:
    Priority(int priority = NORMAL): _priority(priority) {}
    
    enum {
        MAIN   = 0,
        HIGH   = 1,
        NORMAL = (unsigned(1) << (sizeof(int) * 8 - 1)) - 4,
        LOW    = (unsigned(1) << (sizeof(int) * 8 - 1)) - 3,
        IDLE   = (unsigned(1) << (sizeof(int) * 8 - 1)) - 2
    };
    static const bool preemptive = true;

private:
    volatile int _priority;
};

__END_SYS
#endif