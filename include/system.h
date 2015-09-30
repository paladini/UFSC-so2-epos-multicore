// EPOS Global System Abstraction Declarations

#ifndef __system_h
#define __system_h

#include <utility/heap.h>

__BEGIN_SYS

class System
{
    friend class Init_System;
    friend class Init_Application;
    friend void * ::operator new(size_t bytes, const Kernel_Alloc&);
    friend void * ::operator new[](size_t bytes, const Kernel_Alloc&);

public:
    static System_Info<Machine> * const info() { assert(_si); return _si; }

private:
    static void init();

private:
    static System_Info<Machine> * _si;
    static char _preheap[sizeof(Heap)];
    static Heap * _heap;
};

__END_SYS

inline void * operator new(size_t bytes, const Kernel_Alloc&) {
    __USING_SYS
    void * addr = System::_heap->alloc(bytes);
    db<System>(INF) << "System::operator new(bytes=" << bytes << ") => " << addr << endl;
    return addr;
}

inline void * operator new[](size_t bytes, const Kernel_Alloc&) {
    __USING_SYS
    void * addr = System::_heap->alloc(bytes);
    db<System>(INF) << "System::operator new[](bytes=" << bytes << ") => " << addr << endl;
    return addr;
}

#endif
