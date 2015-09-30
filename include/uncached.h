#ifndef __uncached_h
#define __uncached_h

#include <utility/heap.h>
#include <segment.h>

__BEGIN_SYS

class Uncached_Heap
{
    friend class Init_System;
    friend void * ::operator new(size_t, const Uncached_Alloc&);
    friend void * ::operator new[](size_t, const Uncached_Alloc&);

public:
    static void * alloc(unsigned int bytes) { return _heap->alloc(bytes); }

private:
    static Segment * _segment;
    static Heap * _heap;
};

__END_SYS

inline void * operator new(size_t bytes, const Uncached_Alloc&) {
    return _SYS::Uncached_Heap::_heap->alloc(bytes);
}

inline void * operator new[](size_t bytes, const Uncached_Alloc&) {
    return _SYS::Uncached_Heap::_heap->alloc(bytes);
}

#endif
