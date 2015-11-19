#include <Memory.h>


Memory::Memory()
{
    ram = new char[size];
    memset((void *)ram, 0xFF, size);
    rom = new ROM();

}

uint8_t Memory::Read8(uint16_t addr)
{
    if(addr >= 0 && addr <= 0x07FF)
    {
        return ram[addr];
    }
    else if(addr >= 0x0800 && addr <= 0x0FFF)
    {
        return ram[addr - 0x0800];
    }
    else if(addr >= 0x1000 && addr <= 0x17FF)
    {
        return ram[addr - 0x1000];
    }
    else if(addr >= 0x1800 && addr <= 0x1FFF)
    {
        return ram[addr - 0x1800];
    }

    return 0;
}

uint16_t Memory::Read16(uint16_t addr)
{
    return (Read8(addr) | (Read8(addr + 1) << 8));
}

void Memory::Write8(uint16_t addr, uint8_t value)
{
    if(addr >= 0 && addr <= 0x07FF)
    {
        ram[addr] = value;
    }
    else if(addr >= 0x0800 && addr <= 0x0FFF)
    {
        ram[addr - 0x0800] = value;
    }
    else if(addr >= 0x1000 && addr <= 0x17FF)
    {
        ram[addr - 0x1000] = value;
    }
    else if(addr >= 0x1800 && addr <= 0x1FFF)
    {
        ram[addr - 0x1800] = value;
    }
}

void Memory::Write16(uint16_t addr, uint16_t value)
{
    uint8_t lo = (uint8_t)value;
    uint8_t hi = ((value >> 8) & 0xFF);

    Write8(addr, lo);
    Write8(addr + 1, hi);
}


Memory::~Memory()
{
    delete rom;
}
