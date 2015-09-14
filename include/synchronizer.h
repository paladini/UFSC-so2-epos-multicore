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
    Synchronizer_Common(): _wakingAllUp(0) {}

    // Atomic operations
    bool tsl(volatile bool & lock) { return CPU::tsl(lock); }
    int finc(volatile int & number) { return CPU::finc(number); }
    int fdec(volatile int & number) { return CPU::fdec(number); }

    // Thread operations
    void begin_atomic() { Thread::lock(); }
    void end_atomic() { Thread::unlock(); }

    typedef Queue<Thread> ThreadQueue;

    void sleep() {
        begin_atomic();
        Thread * currentThread = Thread::self();
        if (_wakingAllUp == 0) {
            ThreadQueue::Element * element = new (kmalloc(sizeof(ThreadQueue::Element))) ThreadQueue::Element(currentThread);
            _sleeping.insert(element);
            currentThread->suspend();
        } else {
            Thread::yield(); 
        }
    }

    void wakeup() {
        begin_atomic();

        ThreadQueue::Element * sleepingElement = _sleeping.head();
        if (sleepingElement != 0) {
            _sleeping.remove(sleepingElement);
            Thread * sleepingThread = sleepingElement->object();
            delete sleepingElement;
            if (sleepingThread->state() == Thread::SUSPENDED) {
                sleepingThread->resume();
                return;
            }
        }

        end_atomic();
    }

    void wakeup_all() {
        if (_wakingAllUp == 0) {
            finc(_wakingAllUp);
            while (!_sleeping.empty()) {
                wakeup();
            }
            fdec(_wakingAllUp);
        }
    }

private:
    ThreadQueue _sleeping;
    volatile int _wakingAllUp; // Quenio: Evita "race condition" caso wakeup_all() e sleep() sejam chamados concorrentemente.
};

__END_SYS

#endif