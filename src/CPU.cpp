#include "CPU.h"

CPU::CPU()
{
    PC = 0x0600;
    A = 0;
    X = 0;
    Y = 0;
    S = 0xFD;
    P.I = 1;
    P.B = 3;
    m = NULL;
}

void CPU::setPC(uint16_t value)
{
    PC = value;
}

void CPU::setMemory(Memory *_m)
{
    m = _m;
}

void CPU::setRunningState(bool state)
{
    isRunning = state;
}

void CPU::mainLoop(bool debug = false)
{
    while(isRunning)
    {
        if(debug)
            printf("insn %02x\n", m->Read8(PC));

        execute();

        if(debug)
            printState();
    }
}

/* Addressing routines */

uint8_t CPU::immediate_addr() // read only argument of operation
{
    return m->Read8(PC++);
}

int8_t CPU::relative_addr()
{
    return m->Read8(PC++); // like immediate, but int8_t
}

uint8_t CPU::zeropage_addr() // zeropage addressing
{
    uint16_t addr = m->Read8(PC++);
    return m->Read8(addr);
}

uint16_t CPU::zeropage_addr_j() // zeropage which return address in memory
{
    return (uint16_t)m->Read8(PC++);
}

uint16_t CPU::absolute_addr_j() // read only address, needed for jsr, jmp
{
    PC += 2;
    return m->Read16(PC - 2);
}

uint8_t CPU::absolute_addr() // read address arg and read value by it
{
    uint16_t addr = m->Read16(PC);
    PC += 2;
    return m->Read8(addr);
}

uint8_t CPU::zeropage_x_addr()
{
    uint16_t addr = m->Read8(PC++);
    addr += X;
    addr &= 0xff;
    return m->Read8(addr);
}

uint16_t CPU::zeropage_x_addr_j()
{
    uint16_t addr = m->Read8(PC++);
    addr += X;
    addr &= 0xff;
    return addr;
}

uint8_t CPU::zeropage_y_addr()
{
    uint16_t addr = m->Read8(PC++);
    addr += Y;
    addr &= 0xff;
    return m->Read8(addr);
}

uint16_t CPU::zeropage_y_addr_j()
{
    uint16_t addr = m->Read8(PC++);
    addr += Y;
    addr &= 0xff;
    return addr;
}

uint16_t CPU::absolute_x_addr_j()
{
    uint16_t addr = m->Read16(PC);
    PC += 2;
    addr += X;
    return addr;
}

uint8_t CPU::absolute_x_addr()
{
    uint16_t addr = m->Read16(PC);
    PC += 2;
    addr += X;
    return m->Read8(addr);
}

uint16_t CPU::absolute_y_addr_j()
{
    uint16_t addr = m->Read16(PC);
    PC += 2;
    addr += Y;
    return addr;
}

uint8_t CPU::absolute_y_addr()
{
    uint16_t addr = m->Read16(PC);
    PC += 2;
    addr += Y;
    return m->Read8(addr);
}

uint8_t CPU::indexed_indirect_addr() // (MEM, X)
{
    uint16_t addr = m->Read8(PC++);
    addr += X;
    addr &= 0xFF;
    uint16_t new_addr = m->Read16(addr);
    return m->Read8(new_addr);
}

uint8_t CPU::indirect_indexed_addr() // (MEM), Y
{
    uint16_t addr = m->Read8(PC++);
    uint16_t new_addr = m->Read16(addr);
    new_addr += Y;
    return m->Read8(new_addr);
}

uint16_t CPU::indexed_indirect_addr_j() // (MEM, X)
{
    uint16_t addr = m->Read8(PC++);
    addr += X;
    addr &= 0xFF;
    uint16_t new_addr = m->Read16(addr);
    return new_addr;
}

uint16_t CPU::indirect_indexed_addr_j() // (MEM), Y
{
    uint16_t addr = m->Read8(PC++);
    uint16_t new_addr = m->Read16(addr);
    new_addr += Y;
    return new_addr;
}


/* Addressing routines end */

/* Stack routines */

uint8_t CPU::pop8()
{
    uint8_t data = m->Read8(0x0100 + S);
    S++;
    return data;
}

uint16_t CPU::pop16()
{
    uint16_t data = m->Read16(0x0100 + S);
    S += 2;
    return data;
}

void CPU::push8(uint8_t data)
{
    S--;
    m->Write8(0x0100 + S, data);
}

void CPU::push16(uint16_t data)
{
    S -= 2;
    m->Write16(0x0100 + S, data);
}


/* Stack routines end */


void CPU::printState()
{
    printf("A = (0x%02x) | X = (0x%02x) | Y = (0x%02x)\n", A, X, Y);
    printf("PC = (0x%04x) | S = (0x%02x)\n", PC, S);
    printf("FLAGS: N(%d) V(%d) D(%d) B(%d) I(%d) Z(%d) C(%d)\n", P.N, P.V, P.D, P.B, P.I, P.Z, P.C);
}

int CPU::execute()
{
    int cycles = 0;
    uint8_t instr = m->Read8(PC);
    PC++;

    switch(instr)
    {
    case 0x00: // BRK
    {
        isRunning = false; // simply stop our CPU
        break;
    }
    case 0x01:
        break;
    case 0x05:
        break;
    case 0x06:
        break;
    case 0x08:
        break;
    case 0x09: // ORA immediate
    {
        uint8_t arg = immediate_addr();
        A = A | arg;
        if(A == 0)
            P.Z = 1;
        P.N = (A & 128) ? 1 : 0;
        cycles = 2;
        break;
    }
    case 0x0a: // ASL accumulator
    {
        uint8_t newA = A << 1;
        if(newA == 0)
        {
            P.Z = 1;
        }
        P.N = (newA & 128) ? 1 : 0;
        P.C = (A & 128) ? 1 : 0;
        A = newA;
        cycles = 2;
        break;
    }
    case 0x0d:
        break;
    case 0x0e:
        break;
    case 0x10: // BPL
    {
        int8_t offset = relative_addr();
        cycles = 2;
        if(P.N == 0)
        {
            cycles += 1;
            PC = PC + offset; // TODO: check crosspage and add +1 cycle if needed
        }
        break;
    }
    case 0x11:
        break;
    case 0x15:
        break;
    case 0x16:
        break;
    case 0x18: // CLC
    {
        P.C = 0;
        cycles = 2;
        break;
    }
    case 0x19:
        break;
    case 0x1d:
        break;
    case 0x1e:
        break;
    case 0x20: // JSR
    {
        uint16_t newPC = absolute_addr_j();
        push16(PC);
        PC = newPC;
        cycles = 6;
        break;
    }
    case 0x21:
        break;
    case 0x24: // BIT zeropage
    {
        uint8_t value = zeropage_addr();
        if((A & value) == 0)
        {
            P.Z = 0;
        }
        P.N = (value & 128) ? 1 : 0;
        P.V = (value & 64) ? 1 : 0;
        cycles = 3;
        break;
    }
    case 0x25:
        break;
    case 0x26:
        break;
    case 0x28:
        break;
    case 0x29: // AND immediate
    {
        uint8_t arg = immediate_addr();
        A = A & arg;
        if(A == 0)
            P.Z = 1;
        P.N = (A & 128) ? 1 : 0;
        cycles = 2;
        break;
    }
    case 0x2a:
        break;
    case 0x2c: // BIT absolute
    {
        uint8_t value = absolute_addr();
        if((A & value) == 0)
        {
            P.Z = 0;
        }
        P.N = (value & 128) ? 1 : 0;
        P.V = (value & 64) ? 1 : 0;
        cycles = 4;
        break;
    }
    case 0x2d:
        break;
    case 0x2e:
        break;
    case 0x30: // BMI
    {
        int8_t offset = relative_addr();
        cycles = 2;
        if(P.N == 1)
        {
            cycles += 1;
            PC = PC + offset; // TODO: check crosspage
        }
        break;
    }
    case 0x31:
        break;
    case 0x35:
        break;
    case 0x36:
        break;
    case 0x38: // SEC
    {
        P.C = 1;
        cycles = 2;
        break;
    }
    case 0x39:
        break;
    case 0x3d:
        break;
    case 0x3e:
        break;
    case 0x40:
        break;
    case 0x41:
        break;
    case 0x45:
        break;
    case 0x46:
        break;
    case 0x48: // PHA
    {
        push8(A);
        cycles = 3;
        break;
    }
    case 0x49: // EOR immediate
    {
        uint8_t arg = immediate_addr();
        A = A ^ arg;
        if(A == 0)
            P.Z = 1;
        P.N = (A & 128) ? 1 : 0;
        cycles = 2;
        break;
    }
    case 0x4a: // LSR accumulator
    {
        uint8_t newA = A >> 1;
        if(newA == 0)
        {
            P.Z = 1;
        }
        P.N = (newA & 128) ? 1 : 0;
        P.C = (A & 1) ? 1 : 0;
        A = newA;
        cycles = 2;
        break;
    }
    case 0x4c: // JMP absolute
    {
        uint16_t newPC = absolute_addr_j();
        PC = newPC;
        cycles = 3;
        break;
    }
    case 0x4d:
        break;
    case 0x4e:
        break;
    case 0x50: // BVC
    {
        int8_t offset = relative_addr();
        cycles = 2;
        if(P.V == 0)
        {
            cycles += 1;
            PC = PC + offset; // TODO: check crosspage
        }
        break;
    }
    case 0x51:
        break;
    case 0x55:
        break;
    case 0x56:
        break;
    case 0x58: // CLI
    {
        P.I = 0;
        cycles = 2;
        break;
    }
    case 0x59:
        break;
    case 0x5d:
        break;
    case 0x5e:
        break;
    case 0x60: // RTS
    {
        uint16_t newPC = pop16();
        PC = newPC;
        cycles = 6;
        break;
    }
    case 0x61:
        break;
    case 0x65: // ADC zeropage
    {
        uint8_t value = zeropage_addr();
        if(P.D == 0)
        {
            uint16_t result = A + value + P.C;
            A = result & 0xFF;
            if(A == 0)
                P.Z = 1;
            P.N = (result & 128) ? 1 : 0;
            P.V = (((int16_t)result > 0xFF) || ((int16_t)result < 0)) ? 1 : 0;
            P.C = ((result & 256) && P.V) ? 1 : 0;
        }
        cycles = 3;
        break;
    }
    case 0x66:
        break;
    case 0x68: // PLA
    {
        A = pop8();
        cycles = 3;
        break;
    }
    case 0x69: // ADC immediate
    {
        uint8_t value = immediate_addr();
        if(P.D == 0)
        {
            uint16_t result = A + value + P.C;
            A = result & 0xFF;
            if(A == 0)
                P.Z = 1;
            P.N = (result & 128) ? 1 : 0;
            P.V = (((int16_t)result > 0xFF) || ((int16_t)result < 0)) ? 1 : 0;
            P.C = ((result & 256) && P.V) ? 1 : 0;
        }
        cycles = 2;
        break;
    }
    case 0x6a:
        break;
    case 0x6c:
        break;
    case 0x6d: // ADC absolute
    {
        uint8_t value = absolute_addr();
        if(P.D == 0)
        {
            uint16_t result = A + value + P.C;
            A = result & 0xFF;
            if(A == 0)
                P.Z = 1;
            P.N = (result & 128) ? 1 : 0;
            P.V = (((int16_t)result > 0xFF) || ((int16_t)result < 0)) ? 1 : 0;
            P.C = ((result & 256) && P.V) ? 1 : 0;
        }
        cycles = 4;
        break;
    }
    case 0x6e:
        break;
    case 0x70: // BVS
    {
        int8_t offset = relative_addr();
        cycles = 2;
        if(P.V == 1)
        {
            cycles += 1;
            PC = PC + offset; // TODO: check crosspage
        }
        break;
    }
    case 0x71:
        break;
    case 0x75:
        break;
    case 0x76:
        break;
    case 0x78: // SEI
    {
        P.I = 1;
        cycles = 2;
        break;
    }
    case 0x79:
        break;
    case 0x7d:
        break;
    case 0x7e:
        break;
    case 0x81: // STA (indirect, X)
    {
        uint16_t addr = indexed_indirect_addr_j();
        m->Write8(addr, A);
        cycles = 6;
        break;
    }
    case 0x84: // STY zeropage
    {
        uint16_t addr = zeropage_addr_j();
        m->Write8(addr, Y);
        cycles = 3;
        break;
    }
    case 0x85: // STA zeropage
    {
        uint16_t addr = zeropage_addr_j();
        m->Write8(addr, A);
        cycles = 3;
        break;
    }
    case 0x86: // STX zeropage
    {
        uint16_t addr = zeropage_addr_j();
        m->Write8(addr, X);
        cycles = 3;
        break;
    }
    case 0x88: // DEY
    {
        Y--;
        if(Y == 0)
            P.Z = 1;
        P.N = (Y & 128) ? 1 : 0;
        cycles = 2;
        break;
    }
    case 0x8a: // TXA
    {
        A = X;
        if(A == 0)
            P.Z = 1;
        P.N = (A & 128) ? 1 : 0;
        cycles = 2;
        break;
    }
    case 0x8c:
        break;
    case 0x8d:
        break;
    case 0x8e:
        break;
    case 0x90: // BCC
    {
        int8_t offset = relative_addr();
        cycles = 2;
        if(P.C == 0)
        {
            cycles += 1;
            PC = PC + offset; // TODO: check crosspage
        }
        break;
    }
    case 0x91: // STA (indirect), Y
    {
        uint16_t addr = indirect_indexed_addr_j();
        m->Write8(addr, A);
        cycles = 6;
        break;
    }
    case 0x94:
        break;
    case 0x95:
        break;
    case 0x96:
        break;
    case 0x98: // TYA
    {
        A = Y;
        if(A == 0)
            P.Z = 1;
        P.N = (A & 128) ? 1 : 0;
        cycles = 2;
        break;
    }
    case 0x99:
        break;
    case 0x9a: // TXS
    {
        S = X;
        cycles = 2;
        break;
    }
    case 0x9d:
        break;
    case 0xa0:
        break;
    case 0xa1:
        break;
    case 0xa2:
        break;
    case 0xa4: // LDY zeropage
    {
        Y = zeropage_addr();
        if(Y == 0)
            P.Z = 1;
        P.N = (Y & 128) ? 1 : 0;
        cycles = 3;
        break;
    }
    case 0xa5: // LDA zeropage
    {
        A = zeropage_addr();
        if(A == 0)
            P.Z = 1;
        P.N = (A & 128) ? 1 : 0;
        cycles = 3;
        break;
    }
    case 0xa6: // LDX zeropage
    {
        X = zeropage_addr();
        if(X == 0)
            P.Z = 1;
        P.N = (X & 128) ? 1 : 0;
        cycles = 3;
        break;
    }
    case 0xa8: // TAY
    {
        Y = A;
        if(Y == 0)
            P.Z = 1;
        P.N = (Y & 128) ? 1 : 0;
        cycles = 2;
        break;
    }
    case 0xa9: // LDA Immediate
    {
        A = immediate_addr();
        if(A == 0)
            P.Z = 1;
        P.N = (A & 128) ? 1 : 0;
        cycles = 2;
        break;
    }
    case 0xaa: // TAX
    {
        X = A;
        if(X == 0)
            P.Z = 1;
        P.N = (X & 128) ? 1 : 0;
        cycles = 2;
        break;
    }
    case 0xac: // LDY absolute
    {
        Y = absolute_addr();
        if(Y == 0)
            P.Z = 1;
        P.N = (Y & 128) ? 1 : 0;
        cycles = 4;
        break;
    }
    case 0xad: // LDA absolute
    {
        A = absolute_addr();
        if(A == 0)
            P.Z = 1;
        P.N = (A & 128) ? 1 : 0;
        cycles = 4;
        break;
    }
    case 0xae: // LDX absolute
    {
        X = absolute_addr();
        if(X == 0)
            P.Z = 1;
        P.N = (X & 128) ? 1 : 0;
        cycles = 4;
        break;
    }
    case 0xb0: // BCS
    {
        int8_t offset = relative_addr();
        cycles = 2;
        if(P.C == 1)
        {
            cycles += 1;
            PC = PC + offset; // TODO: check crosspage
        }
        break;
    }
    case 0xb1:
        break;
    case 0xb4:
        break;
    case 0xb5:
        break;
    case 0xb6:
        break;
    case 0xb8: // CLV
    {
        P.V = 0;
        cycles = 2;
        break;
    }
    case 0xb9:
        break;
    case 0xba: // TSX
    {
        X = S;
        if(X == 0)
            P.Z = 1;
        P.N = (X & 128) ? 1 : 0;
        cycles = 2;
        break;
    }
    case 0xbc:
        break;
    case 0xbd:
        break;
    case 0xbe:
        break;
    case 0xc0:
        break;
    case 0xc1:
        break;
    case 0xc4:
        break;
    case 0xc5: // CMP zeropage
    {
        int8_t arg = (int8_t)zeropage_addr();
        if((int8_t)A >= arg)
        {
            P.C = 1;
        }
        if((int8_t)A == arg)
        {
            P.Z = 1;
        }
        if((int8_t)A < arg)
        {
            P.N = 1;
        }
        cycles = 3;
        break;
    }
    case 0xc6: // DEC zeropage
    {
        uint16_t addr = zeropage_addr_j();
        uint8_t value = m->Read8(addr);
        value--;
        if(value == 0)
        {
            P.Z = 1;
        }
        P.N = (value & 128) ? 1 : 0;
        m->Write8(addr, value);
        cycles = 5;
        break;
    }
    case 0xc8: // INY
    {
        Y++;
        if(Y == 0)
            P.Z = 1;
        P.N = (Y & 128) ? 1 : 0;
        cycles = 2;
        break;
    }
    case 0xc9: // CMP immediate
    {
        int8_t arg = (int8_t)immediate_addr();
        if((int8_t)A >= arg)
        {
            P.C = 1;
        }
        if((int8_t)A == arg)
        {
            P.Z = 1;
        }
        if((int8_t)A < arg)
        {
            P.N = 1;
        }
        cycles = 2;
        break;
    }
    case 0xca: // DEX
    {
        X--;
        if(X == 0)
            P.Z = 1;
        P.N = (X & 128) ? 1 : 0;
        cycles = 2;
        break;
    }
    case 0xcc:
        break;
    case 0xcd:
        break;
    case 0xce: // DEC absolute
    {
        uint16_t addr = absolute_addr_j();
        uint8_t value = m->Read8(addr);
        value--;
        if(value == 0)
        {
            P.Z = 1;
        }
        P.N = (value & 128) ? 1 : 0;
        m->Write8(addr, value);
        cycles = 6;
        break;
    }
    case 0xd0: // BNE
    {
        int8_t offset = relative_addr();
        cycles = 2;
        if(P.Z == 0)
        {
            cycles += 1;
            PC = PC + offset; // TODO: check crosspage
        }
        break;
    }
    case 0xd1:
        break;
    case 0xd5:
        break;
    case 0xd6:
        break;
    case 0xd8: // CLD
    {
        P.D = 0;
        cycles = 2;
        break;
    }
    case 0xd9:
        break;
    case 0xdd:
        break;
    case 0xde:
        break;
    case 0xe0:
        break;
    case 0xe1:
        break;
    case 0xe4:
        break;
    case 0xe5:
        break;
    case 0xe6: // INC zeropage
    {
        uint16_t addr = zeropage_addr_j();
        uint8_t value = m->Read8(addr);
        value++;
        if(value == 0)
        {
            P.Z = 1;
        }
        P.N = (value & 128) ? 1 : 0;
        m->Write8(addr, value);
        cycles = 5;
        break;
    }
    case 0xe8: // INX
    {
        X++;
        if(X == 0)
            P.Z = 1;
        P.N = (X & 128) ? 1 : 0;
        cycles = 2;
        break;
    }
    case 0xe9: // SBC immediate
    {
        uint8_t value = immediate_addr();
        if(P.D == 0)
        {
            uint16_t result = A - value - (1 - P.C);
            A = result & 0xFF;
            if(A == 0)
                P.Z = 1;
            P.N = (result & 128) ? 1 : 0;
            P.V = (((int16_t)result > 0xFF) || ((int16_t)result < 0)) ? 1 : 0; // shiiiiiiit
            P.C = ((result & 256) && P.V) ? 1 : 0; // oh my god
        }
        cycles = 2;
        break;
    }
    case 0xea: // NOP
    {
        cycles = 2;
        break;
    }
    case 0xec:
        break;
    case 0xed: // SBC absolute
    {
        uint8_t value = absolute_addr();
        if(P.D == 0)
        {
            uint16_t result = A - value - (1 - P.C);
            A = result & 0xFF;
            if(A == 0)
                P.Z = 1;
            P.N = (result & 128) ? 1 : 0;
            P.V = (((int16_t)result > 0xFF) || ((int16_t)result < 0)) ? 1 : 0; // i think this is incorrect
            P.C = ((result & 256) && P.V) ? 1 : 0; // and this too
        }
        cycles = 2;
        break;
    }
    case 0xee: // INC absolute
    {
        uint16_t addr = absolute_addr_j();
        uint8_t value = m->Read8(addr);
        value++;
        if(value == 0)
        {
            P.Z = 1;
        }
        P.N = (value & 128) ? 1 : 0;
        m->Write8(addr, value);
        cycles = 6;
        break;
    }
    case 0xf0: // BEQ
    {
        int8_t offset = relative_addr();
        cycles = 2;
        if(P.Z == 1)
        {
            cycles += 1;
            PC = PC + offset; // TODO: check crosspage
        }
        break;
    }
    case 0xf1:
        break;
    case 0xf5:
        break;
    case 0xf6: // INC zeropage,X
        break;
    case 0xf8: // SED
    {
        P.D = 1;
        cycles = 2;
        break;
    }
    case 0xf9:
        break;
    case 0xfd:
        break;
    case 0xfe:
        break;

    default:
        printf("Unknown opcode at 0x%04x!\n", PC-1);
        isRunning = false; // unknown opcode will stop CPU
        break;
    }

    return cycles;
}

CPU::~CPU()
{

}
