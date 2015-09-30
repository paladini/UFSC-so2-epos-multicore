// EPOS Application Scaffold and Application Abstraction Implementation

#include <utility/ostream.h>
#include <application.h>

__BEGIN_SYS

// Application class attributes
char Application::_preheap[];
Segment * Application::_heap_segment;
Heap * Application::_heap;
char Application::_preuncached[];
Segment * Application::_uncached_segment;
Heap * Application::_uncached;

__END_SYS

__BEGIN_API

// Global objects
__USING_UTIL
OStream cout;
OStream cerr;

__END_API
