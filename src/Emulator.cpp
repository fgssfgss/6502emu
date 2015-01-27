#include "Emulator.h"

Emulator::Emulator()
{
}

int Emulator::run()
{
    cpu.setMemory(&mem);

    return 0;
}

Emulator::~Emulator()
{
}
