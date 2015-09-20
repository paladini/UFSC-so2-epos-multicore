// EPOS Handler Utility Declarations

#ifndef __handler_h
#define __handler_h

#include <system/config.h>

__BEGIN_UTIL

class Handler
{
public:
    Handler() {}
    virtual void operator()() = 0;
};

__END_UTIL

#endif
