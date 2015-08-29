// EPOS Configuration Engine

#ifndef __config_h
#define __config_h

//============================================================================
// ARCHITECTURE, MACHINE, AND APPLICATION SELECTION
// This section is generated automatically from makedefs by $EPOS/etc/makefile
//============================================================================
#define MODE xxx
#define ARCH xxx
#define MACH xxx
#define MMOD xxx
#define APPL xxx
#define __mode_xxx__
#define __arch_xxx__
#define __mach_xxx__
#define __mmod_xxx__

#if defined (__arch_avr__) || defined (__mmod_lm3s811__)
#define __no_networking__
#endif

//============================================================================
// NAMESPACES AND DEFINITIONS
//============================================================================
namespace EPOS {
    namespace S {
        namespace U {}
        using namespace U;
    }
}

#define __BEGIN_API             namespace EPOS {
#define __END_API               }
#define _API                    ::EPOS

#define __BEGIN_UTIL            namespace EPOS { namespace S { namespace U {
#define __END_UTIL              }}}
#define __USING_UTIL            using namespace S::U;
#define _UTIL                   ::EPOS::S::U

#define __BEGIN_SYS             namespace EPOS { namespace S {
#define __END_SYS               }}
#define __USING_SYS             using namespace EPOS::S;
#define _SYS                    ::EPOS::S

#ifndef __mode_kernel__
namespace EPOS {
    using namespace S;
    using namespace S::U;
}
#endif

#define __HEADER_ARCH(X)        <architecture/ARCH/X.h>
#define __HEADER_MACH(X)        <machine/MACH/X.h>
#define __MACH_TRAITS_T(X)      <machine/MACH/X##_traits.h>
#define __MACH_TRAITS(X)        __MACH_TRAITS_T(X)
#define __MACH_CONFIG_T(X)      <machine/MACH/X##_config.h>
#define __MACH_CONFIG(X)        __MACH_CONFIG_T(X)
#define __APPL_TRAITS_T(X)      <../app/X##_traits.h>
#define __APPL_TRAITS(X)        __APPL_TRAITS_T(X)

#define __ARCH_TRAITS_H         __HEADER_ARCH(traits)
#define __MACH_TRAITS_H         __MACH_TRAITS(MMOD)
#define __MACH_CONFIG_H         __MACH_CONFIG(MMOD)
#define __APPL_TRAITS_H         __APPL_TRAITS(APPL)

#define ASM                     __asm__ __volatile__

//============================================================================
// ASSERT (for pre and post conditions)
//============================================================================
//#define assert(expr)    ((expr) ? static_cast<void>(0) : Assert::fail (#expr, __FILE__, __LINE__, __PRETTY_FUNCTION__))
#define assert(expr)    (static_cast<void>(0))

//============================================================================
// CONFIGURATION
//============================================================================
#include <system/types.h>
#include <system/meta.h>
#include __APPL_TRAITS_H
#include <system/info.h>

//============================================================================
// THINGS EVERBODY NEEDS
//============================================================================
#include <utility/ostream.h>
#include <utility/debug.h>

#endif
