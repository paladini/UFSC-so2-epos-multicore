// EPOS Application Initializer

#include <utility/heap.h>
#include <mmu.h>
#include <machine.h>
#include <application.h>
#include <address_space.h>

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

		Segment* seg = new (&Application::_preheap[0]) Segment(HEAP_SIZE);
		Application::_heap = new (&Application::_preheap[sizeof(Segment)]) Heap(Address_Space(MMU::current()).attach(seg), seg->size());

		Segment* seg_uncached = new (&Application::_uncached_preheap[0]) Segment(HEAP_SIZE, (Segment::Flags::APP | Segment::Flags::CWT));
		Application::_uncached_heap = new (&Application::_uncached_preheap[sizeof(Segment)]) Heap(Address_Space(MMU::current()).attach(seg_uncached), seg_uncached->size());

		db<Init>(INF) << "done!" << endl;
    }
};

// Global object "init_application"  must be linked to the application (not
// to the system) and there constructed at first.
Init_Application init_application;

__END_SYS
