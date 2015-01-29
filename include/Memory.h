#ifndef MEMORY_H
#define MEMORY_H
#include <cstdint>
#include <ROM.h>
#include <cstring>

class Memory
{
    public:
        Memory();
        virtual ~Memory();
        uint8_t Read8(uint16_t addr);
        uint16_t Read16(uint16_t addr);
        void Write8(uint16_t addr, uint8_t value);
        void Write16(uint16_t addr, uint16_t value);
    protected:
    private:
        const size_t size = 2048;
        char *ram; // 2048 bytes
        ROM *rom;
};

#endif // MEMORY_H
