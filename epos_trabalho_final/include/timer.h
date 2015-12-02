// EPOS Timer Mediator Common Package

#ifndef __timer_h
#define __timer_h

#include <tsc.h>
#include <ic.h>

__BEGIN_SYS

class Timer_Common
{
protected:
    Timer_Common() {}

public:
    typedef TSC::Hertz Hertz;
    typedef TSC::Hertz Tick;
    typedef IC::Interrupt_Handler Handler;
    typedef unsigned int Channel;
};

__END_SYS

#ifdef __TIMER_H
#include __TIMER_H
#endif

#endif
