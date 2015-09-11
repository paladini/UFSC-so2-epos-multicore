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
    ~Synchronizer_Common() {
        begin_atomic();
        wakeup_all();
        end_atomic();
    }

    // Atomic operations
    bool tsl(volatile bool & lock) { return CPU::tsl(lock); }
    int finc(volatile int & number) { return CPU::finc(number); }
    int fdec(volatile int & number) { return CPU::fdec(number); }

    // Thread operations
    void begin_atomic() { Thread::lock(); }
    void end_atomic() { Thread::unlock(); }

    void sleep() { 
        begin_atomic();

        Thread* previous = Thread::running();
        previous->_state = Thread::WAITING;  
        queue.insert(&previous->_link);

        //if(!Thread::_ready.empty()) {
            Thread::_running = Thread::_ready.remove()->object();
            Thread::_running->_state = Thread::RUNNING;
            Thread::dispatch(previous, Thread::_running);
       //} else {
       //     Thread::idle();
       // }

        end_atomic();
    } 
    void wakeup() { 
        begin_atomic();

        if(!queue.empty()) {
            wakeupThread();
        }

        end_atomic();

        if(Thread::preemptive)
            Thread::reschedule();
    }
    void wakeup_all() { 
        begin_atomic();

        while(!queue.empty()) {
            wakeupThread();
        }

        end_atomic();

        if(Thread::preemptive)
            Thread::reschedule();
    }

    void wakeupThread(){
        Thread* syncThread = queue.remove()->object();
        syncThread->_state = Thread::READY;
        Thread::_ready.insert(&syncThread->_link);
    }

private:
    Thread::Queue queue;
};

__END_SYS

#endif

