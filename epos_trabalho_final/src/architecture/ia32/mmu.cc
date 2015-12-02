// EPOS IA32 MMU Mediator Implementation

#include <architecture/ia32/mmu.h>

__BEGIN_SYS

// Class attributes
IA32_MMU::List IA32_MMU::_free;
IA32_MMU::Page_Directory * IA32_MMU::_master;

__END_SYS
