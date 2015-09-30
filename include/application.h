// EPOS Global Application Abstraction Declarations

#ifndef __application_h
#define __application_h

#include <utility/heap.h>
#include <segment.h>

extern "C"
{
    void * malloc(size_t);
    void free(void *);
}

__BEGIN_SYS

class Application
{
    friend class Init_Application;
    friend void * ::malloc(size_t);
    friend void ::free(void *);
    friend void * ::operator new(size_t, const Uncached_Alloc&);
    friend void * ::operator new[](size_t, const Uncached_Alloc&);

private:
    static void init();

private:
    // allocate space for both cached and uncached heap
    static char _preheap[sizeof(Segment) + sizeof(Heap)];
    static Segment * _heap_segment;
    static Heap * _heap;

    // uncached heap shit
    static char _preuncached[sizeof(Segment) + sizeof(Heap)];
    static Segment * _uncached_segment;
    static Heap * _uncached;
};

__END_SYS

inline void * operator new(size_t bytes, const Uncached_Alloc&) {
    return _SYS::Application::_uncached->alloc(bytes);
}

inline void * operator new[](size_t bytes, const Uncached_Alloc&) {
    return _SYS::Application::_uncached->alloc(bytes);
}

#include <utility/malloc.h>

#endif
