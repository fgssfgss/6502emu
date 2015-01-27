#ifndef CPU_H
#define CPU_H
#include <cstdint>
#include <Memory.h>
#include <cstdio>

class CPU
{
    public:
        CPU();
        virtual ~CPU();
        int execute(); //
        void setMemory(Memory *_m);
        void setPC(uint16_t value);
    protected:
    private:
        Memory *m;

        uint16_t PC;
        uint8_t A;
        uint8_t X;
        uint8_t Y;
        uint8_t S;
        struct {
            uint8_t N: 1;
            uint8_t V: 1;
            uint8_t B: 2;
            uint8_t D: 1;
            uint8_t I: 1;
            uint8_t Z: 1;
            uint8_t C: 1;
        } P;
};

#endif // CPU_H
