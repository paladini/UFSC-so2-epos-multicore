// EPOS System-level Dynamic Memory Declarations

#ifndef __kmalloc_h
#define __kmalloc_h

#include <system.h>

__BEGIN_SYS

inline void * kmalloc(size_t bytes) {
    return System::_heap->alloc(bytes);
}

inline void kfree(void * ptr) {
    System::_heap->free(ptr);
}

__END_SYS

#endif
