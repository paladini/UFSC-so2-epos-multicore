// EPOS Synchronizer Abstractions Common Package

#ifndef __synchronizer_h
#define __synchronizer_h

#include <cpu.h>
#include <thread.h>
#include <utility/queue.h>

__BEGIN_SYS

class Synchronizer_Common
{
protected:
    Synchronizer_Common() {}

    // não consegui pensar em outra coisa que não usar essa variável, mas é uma solução paliativa.
    Thread* temp; 

    // Atomic operations
    bool tsl(volatile bool & lock) { return CPU::tsl(lock); }
    int finc(volatile int & number) { return CPU::finc(number); }
    int fdec(volatile int & number) { return CPU::fdec(number); }

    // Thread operations
    void begin_atomic() { Thread::lock(); }
    void end_atomic() { Thread::unlock(); }

    // Guto disse que não seria só isso, mas que é um bom começo.
    void sleep() 
    { 
        //Thread* running = Thread::running();
        //running->suspend();
        temp = Thread::running();
        temp->suspend();
        //Thread::yield(); 
    } // implicit unlock()
    
    void wakeup() 
    {
        temp->resume(); // guto deu aviso para tomar cuidado com esse método.
        end_atomic(); 
    }

    void wakeup_all() 
    {
        for(unsigned int i = 0; i < Thread::_suspended.size(); i++) {
            Thread::Queue::Element* suspended = Thread::_suspended.head();
            suspended->object()->resume();   
        }
        end_atomic(); 
    }
};

__END_SYS

#endif

