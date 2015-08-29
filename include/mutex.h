// EPOS Mutex Abstraction Declarations

#ifndef __mutex_h
#define __mutex_h

#include <semaphore.h>
#include <synchronizer.h>

__BEGIN_SYS

class Mutex: protected Synchronizer_Common
{
public:
    Mutex();
    ~Mutex();

    void lock();
    void unlock();

private:
    Semaphore sem;
};

__END_SYS

#endif
