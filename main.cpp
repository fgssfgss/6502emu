#include <iostream>
#include <Emulator.h>

using namespace std;

int main() {
    Emulator *emu = new Emulator();
    int ret = emu->run();
    delete emu;
    return ret;
}
