// EPOS First Thread Initializer

#include <utility/heap.h>
#include <system.h>
#include <thread.h>
#include <alarm.h> // for FCFS

extern "C" { void __epos_app_entry(); }

__BEGIN_SYS

class Init_First
{
private:
    typedef CPU::Log_Addr Log_Addr;

public:
    Init_First() {

        db<Init>(TRC) << "Init_First()" << endl;

        if(!Traits<System>::multithread) {
            CPU::int_enable();
            return;
        }

        db<Init>(INF) << "Initializing the first thread: " << endl;

        // If EPOS is not a kernel, then adjust the application entry point to __epos_app_entry,
        // which will directly call main(). In this case, _init will have already been called,
        // before Init_Application, to construct main()'s global objects.
        Thread * first = new (SYSTEM) Thread(Thread::Configuration(Thread::RUNNING, Thread::MAIN), reinterpret_cast<int (*)()>(__epos_app_entry));

        // Idle thread creation must succeed main, thus avoiding implicit rescheduling
        new (SYSTEM) Thread(Thread::Configuration(Thread::READY, Thread::IDLE), &Thread::idle);

        db<Init>(INF) << "done!" << endl;

        db<Init>(INF) << "INIT ends here!" << endl;

        db<Init, Thread>(INF) << "Dispatching the first thread: " << first << endl;

        This_Thread::not_booting();

        first->_context->load();
    }
};

// Global object "init_first" must be constructed last in the context of the
// OS, for it activates the first application thread (usually main()) 
Init_First init_first;

__END_SYS
