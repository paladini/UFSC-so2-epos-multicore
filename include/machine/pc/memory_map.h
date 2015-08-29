// EPOS PC Memory Map

#ifndef __pc_memory_map_h
#define __pc_memory_map_h

#include <system/memory_map.h>

__BEGIN_SYS

template<>
struct Memory_Map<PC>
{
    // Physical Memory
    enum {
        MEM_BASE =	Traits<PC>::MEM_BASE,
        MEM_TOP =	Traits<PC>::MEM_TOP
    };

    // Logical Address Space
    enum {
        APP_LOW =       Traits<PC>::APP_LOW,
        APP_CODE =      Traits<PC>::APP_CODE,
        APP_DATA =      Traits<PC>::APP_DATA,
        APP_HIGH =      Traits<PC>::APP_HIGH,

        PHY_MEM =       Traits<PC>::PHY_MEM,
        IO =            Traits<PC>::IO_BASE,
        APIC =          IO,
        VGA =           IO +  4 * 1024,
        PCI =           IO + 68 * 1024,

        SYS =           Traits<PC>::SYS,
        IDT =           SYS + 0x00000000,
        GDT =           SYS + 0x00001000,
        SYS_PT =        SYS + 0x00002000,
        SYS_PD =        SYS + 0x00003000,
        SYS_INFO =      SYS + 0x00004000,
        TSS0 =          SYS + 0x00005000,
        SYS_CODE =      SYS + 0x00300000,
        SYS_DATA =      SYS + 0x00340000,
        SYS_STACK =     SYS + 0x003c0000,
        SYS_HEAP =      SYS + 0x00400000
    };
};

__END_SYS

#endif
