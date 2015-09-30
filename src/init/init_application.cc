// EPOS Application Initializer

#include <utility/heap.h>
#include <mmu.h>
#include <machine.h>
#include <application.h>
#include <address_space.h>
#include <system.h>

__BEGIN_SYS

class Init_Application
{
private:
    static const unsigned int HEAP_SIZE = Traits<Application>::HEAP_SIZE;

public:
    Init_Application() {
        db<Init>(TRC) << "Init_Application()" << endl;

	// Initialize Application's heap
	db<Init>(INF) << "Initializing application's heap" << endl;

    typedef Segment::Flags Flags;
    Address_Space self(MMU::current());

    // cached
    {
        Application::_heap_segment = new (&Application::_preheap[0]) Segment(HEAP_SIZE, Flags::APP);
        CPU::Log_Addr * addr = self.attach(Application::_heap_segment);

        Application::_heap = new (&Application::_preheap[sizeof(Segment)]) Heap(addr, Application::_heap_segment->size());
    }

    // uncached
    {
        Application::_uncached_segment = new (&Application::_preuncached[0]) Segment(HEAP_SIZE, Flags::APP | Flags::CD);
        CPU::Log_Addr * addr = self.attach(Application::_heap_segment);

        Application::_uncached = new (&Application::_preuncached[sizeof(Segment)]) Heap(addr, Application::_uncached_segment->size());
    }

    db<Init>(INF) << "done!" << endl;
    }
};

// Global object "init_application"  must be linked to the application (not
// to the system) and there constructed at first.
Init_Application init_application;

__END_SYS
