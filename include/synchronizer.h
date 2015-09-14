// EPOS Synchronizer Abstractions Common Package
// Supostamente uma queue diferente pra mutex e pra semaphore? 

#ifndef __synchronizer_h
#define __synchronizer_h

#include <cpu.h>
#include <thread.h>

__BEGIN_SYS

class Synchronizer_Common
{
protected:
    Synchronizer_Common() {}

    // Atomic operations
    bool tsl(volatile bool & lock) { return CPU::tsl(lock); }
    int finc(volatile int & number) { return CPU::finc(number); }
    int fdec(volatile int & number) { return CPU::fdec(number); }

    // Thread operations
    void begin_atomic() { Thread::lock(); }
    void end_atomic() { Thread::unlock(); }

    void sleep() {
        Thread::sleep(_queue);
    }

    void wakeup() {
        Thread::wakeup(_queue);
    }

    void wakeup_all() {
        Thread::wakeup_all(_queue);
    }

private:
    Thread::Queue _queue;
};

__END_SYS

#endif
