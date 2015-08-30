// EPOS Synchronizer Abstractions Common Package

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

    void sleep() 
    { 
        Thread* to_be_suspended = Thread::running();
        _temp.insert(&to_be_suspended->_link);
        Thread::running()->suspend();
    } // implicit unlock()
    
    void wakeup() 
    {
        if (Thread::Queue::Element* suspended = _temp.remove()) {
            suspended->object()->resume();
        }
        Thread::reschedule();
    }

    void wakeup_all() 
    {
        for (unsigned int i = 0; i < _temp.size(); ++i) {
            // Thread::Queue::Element* suspended = temp.head();
            _temp.remove()->object()->resume();   
        }
        Thread::reschedule();
    }

private:
    Thread::Queue _temp;
};

__END_SYS

#endif

