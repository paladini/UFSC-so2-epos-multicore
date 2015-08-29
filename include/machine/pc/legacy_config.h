// EPOS PC Mediators Configuration

#ifndef __pc_config_h
#define __pc_config_h

#include <system/meta.h>
#include __APPL_TRAITS_H

#define __CPU_H         __HEADER_ARCH(cpu)
#define __TSC_H         __HEADER_ARCH(tsc)
#define __MMU_H         __HEADER_ARCH(mmu)

#define __MACH_H        __HEADER_MACH(machine)
#define __PCI_H         __HEADER_MACH(pci)
#define __IC_H          __HEADER_MACH(ic)
#define __TIMER_H       __HEADER_MACH(timer)
#define __RTC_H         __HEADER_MACH(rtc)
#define __EEPROM_H      __HEADER_MACH(eeprom)
#define __UART_H        __HEADER_MACH(uart)
#define __DISPLAY_H     __HEADER_MACH(display)
#define __NIC_H         __HEADER_MACH(nic)

__BEGIN_SYS

typedef IA32            CPU;
typedef IA32_MMU        MMU;
typedef IA32_TSC        TSC;

typedef PC              Machine;
typedef PC_PCI          PCI;
typedef PC_IC           IC;
typedef PC_Timer        Timer;
typedef PC_RTC          RTC;
typedef PC_EEPROM       EEPROM;
typedef PC_UART         UART;
typedef IF<Traits<Serial_Display>::enabled, Serial_Display, PC_Display>::Result Display;

__END_SYS

#endif
