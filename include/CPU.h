#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <Memory.h>
#include <cstdio>

class CPU {
public:
    CPU();

    virtual ~CPU();

    int execute();

    void setMemory(Memory *_m);

    void printState();

    void setPC(uint16_t value);

    void mainLoop(bool debug);

    void setRunningState(bool state);

protected:
private:
    Memory *m;
    bool isRunning;

    uint8_t immediate_addr();

    int8_t relative_addr();

    uint8_t zeropage_addr();

    uint16_t zeropage_addr_j();

    uint16_t absolute_addr_j();

    uint8_t absolute_addr();

    uint8_t zeropage_x_addr();

    uint16_t zeropage_x_addr_j();

    uint8_t zeropage_y_addr();

    uint16_t zeropage_y_addr_j();

    uint16_t absolute_x_addr_j();

    uint8_t absolute_x_addr();

    uint16_t absolute_y_addr_j();

    uint8_t absolute_y_addr();

    uint8_t indexed_indirect_addr();

    uint8_t indirect_indexed_addr();

    uint16_t indexed_indirect_addr_j();

    uint16_t indirect_indexed_addr_j();

    uint8_t pop8();

    uint16_t pop16();

    void push8(uint8_t data);

    void push16(uint16_t data);

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
