// EPOS System Scaffold and System Abstraction Implementation

#include <utility/ostream.h>
#include <utility/heap.h>
#include <display.h>
#include <machine.h>
#include <system.h>
#include <thread.h>

__BEGIN_SYS

// This class purpose is simply to define a well-known entry point for
// the system. It must be declared as the first global object in
// system_scaffold.cc
class First_Object
{
public:
    First_Object() {
	Display::init();
    }
};

// Global objects
// These objects might be reconstructed several times in SMP configurations,
// so their constructors must be stateless!
First_Object __entry;
OStream kout;
OStream kerr;

// System class attributes
System_Info<Machine> * System::_si = reinterpret_cast<System_Info<Machine> *>(Memory_Map<Machine>::SYS_INFO);
char System::_preheap[];
Heap * System::_heap;

__END_SYS

// Bindings
__USING_SYS;
extern "C" {
    void _panic() { Machine::panic(); }
    void _exit(int s) { Thread::exit(s); }
    void __exit() { Thread::exit(CPU::fr()); }
    void _print(const char * s) { Display::puts(s); }

    // LIBC Heritage
    void __cxa_pure_virtual() {
        db<void>(ERR) << "Pure Virtual method called!" << endl;
    }
}
