// EPOS Application-level Dynamic Memory Utility Declarations

#ifndef __malloc_h
#define __malloc_h

#include <utility/string.h>
#include <application.h>

extern "C"
{
    // Standard C Library allocators
    inline void * malloc(size_t bytes) {
        __USING_SYS;
	return Application::_heap->alloc(bytes);
    }

    inline void * calloc(size_t n, unsigned int bytes) {
        void * ptr = malloc(n * bytes);
        memset(ptr, 0, n * bytes);
        return ptr;
    }

    inline void free(void * ptr) {
        __USING_SYS;
        Segment * segment = Application::_heap_segment;
        if (ptr >= segment->phy_address() && ptr < (segment->phy_address() + segment->size())) {
            Application::_heap->free(ptr);
            return;
        }

        segment = Application::_uncached_segment;
        if (ptr >= segment->phy_address() && ptr < (segment->phy_address() + segment->size())) {
            Application::_uncached->free(ptr);
            return;
        }
    }
}

// C++ dynamic memory allocators and deallocators
inline void * operator new(size_t bytes) {
    return malloc(bytes);
}

inline void * operator new[](size_t bytes) {
    return malloc(bytes);
}

// Delete cannot be declared inline due to virtual destructors
void operator delete(void * ptr);
void operator delete[](void * ptr);

#endif
