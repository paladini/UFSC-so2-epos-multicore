// EPOS Memory Segment Abstraction Declarations

#ifndef __segment_h
#define __segment_h

#include <mmu.h>

__BEGIN_SYS

class Segment: public MMU::Chunk
{
private:
    typedef MMU::Chunk Chunk;

public:
    typedef MMU::Flags Flags;
    typedef CPU::Phy_Addr Phy_Addr;

public:
    Segment(unsigned int bytes, Flags flags = Flags::APP);
    Segment(Phy_Addr phy_addr, unsigned int bytes, Flags flags);
    ~Segment();

    unsigned int size() const;
    Phy_Addr phy_address() const;
    int resize(int amount);
};

__END_SYS

#endif
