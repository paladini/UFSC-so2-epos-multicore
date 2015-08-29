// EPOS PC Mediator Implementation

#include <machine/pc/machine.h>

__BEGIN_SYS

// Class attributes
volatile unsigned int PC::_n_cpus;

// Class methods
void PC::panic()
{
    CPU::int_disable(); 
    Display::position(24, 73);
    Display::puts("PANIC!");
    if(Traits<System>::reboot)
        Machine::reboot();
    else
        CPU::halt();
}

void PC::reboot()
{
    for(int i = 0; (i < 300) && (CPU::in8(0x64) & 0x02); i++)
        i8255::ms_delay(1);

    // Sending 0xfe to the keyboard controller port causes it to pulse
    // the reset line
    CPU::out8(0x64, 0xfe);

    for(int i = 0; (i < 300) && (CPU::in8(0x64) & 0x02); i++)
        i8255::ms_delay(1);
}

__END_SYS
