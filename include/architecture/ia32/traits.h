// EPOS IA32 Architecture Metainfo
#ifndef __ia32_traits_h
#define __ia32_traits_h

#include <system/config.h>

__BEGIN_SYS

template<> struct Traits<IA32>: public Traits<void>
{
    enum {LITTLE, BIG};
    static const unsigned int ENDIANESS         = LITTLE;
    static const unsigned int WORD_SIZE         = 32;
    static const unsigned int CLOCK             = 2000000000;
    static const bool unaligned_memory_access   = true;
};

template<> struct Traits<IA32_TSC>: public Traits<void>
{
};

template<> struct Traits<IA32_MMU>: public Traits<void>
{
};

template<> struct Traits<IA32_PMU>: public Traits<void>
{
};

__END_SYS

#endif
