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
    friend void * ::operator new(size_t, const EPOS::Heap_Uncached&);
    friend void * ::operator new[](size_t, const EPOS::Heap_Uncached&);

private:
    static void init();

private:
    static char _preheap[sizeof(Segment) + sizeof(Heap)];
    static Heap * _heap;
    static char _uncached_preheap[sizeof(Segment) + sizeof(Heap)];
    static Heap* _uncached_heap;
};

__END_SYS

inline void * operator new(size_t bytes, const EPOS::Heap_Uncached&) {
	return EPOS::Application::_uncached_heap->alloc(bytes);
}
inline void * operator new[](size_t bytes, const EPOS::Heap_Uncached&) {
	return EPOS::Application::_uncached_heap->alloc(bytes);
}

#endif
