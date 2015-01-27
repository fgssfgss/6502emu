#ifndef EMULATOR_H
#define EMULATOR_H

#include <CPU.h>
#include <Memory.h>

class Emulator
{
    public:
        Emulator();
        int run();
        virtual ~Emulator();
    protected:
    private:
        CPU cpu;
        Memory mem;
};

#endif // EMULATOR_H
