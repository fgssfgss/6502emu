#include <Emulator.h>

Emulator::Emulator() {
}

int Emulator::run() {
    cpu.setMemory(&mem);
    cpu.setRunningState(true);
    cpu.mainLoop(true);
    return 0;
}

Emulator::~Emulator() {
}
