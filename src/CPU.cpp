#include "CPU.h"

CPU::CPU()
{
    PC = 0;
    A = 0;
    X = 0;
    Y = 0;
    S = 0xFD;
    P.I = 1;
    P.B = 3;
}

void CPU::setPC(uint16_t value)
{
    PC = value;
}

void CPU::setMemory(Memory *_m)
{
    m = _m;
}

int CPU::execute()
{
    int cycles = 0;
    uint8_t instr = m->Read8(PC);
    PC++;

    switch(instr)
    {
    case 0x00: // BRK
        break;
    case 0x01:
        break;
    case 0x05:
        break;
    case 0x06:
        break;
    case 0x08:
        break;
    case 0x09:
        break;
    case 0x0a:
        break;
    case 0x0d:
        break;
    case 0x0e:
        break;
    case 0x10: // BPL
    {
        int8_t offset = m->Read8(PC);
        PC++;
        cycles = 2;
        if(P.N == 0)
        {
            cycles += 1;
            PC = PC + offset; // TODO: check crosspage
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
        uint16_t newPC = m->Read16(PC);
        PC += 2;
        S -= 2;
        m->Write16(0x0100 + S, PC);
        PC = newPC;
        cycles = 6;
        break;
    }
    case 0x21:
        break;
    case 0x24:
        break;
    case 0x25:
        break;
    case 0x26:
        break;
    case 0x28:
        break;
    case 0x29: // AND immediate
    {
        uint8_t arg = m->Read8(PC);
        PC++;
        A = A & arg;
        if(A == 0)
            P.Z = 1;
        P.N = (A & 128) ? 1 : 0;
        cycles = 2;
        break;
    }
    case 0x2a:
        break;
    case 0x2c:
        break;
    case 0x2d:
        break;
    case 0x2e:
        break;
    case 0x30: // BMI
    {
        int8_t offset = m->Read8(PC);
        PC++;
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
    case 0x48:
        break;
    case 0x49:
        break;
    case 0x4a:
        break;
    case 0x4c: // JMP absolute
    {
        uint16_t newPC = m->Read16(PC);
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
        int8_t offset = m->Read8(PC);
        PC++;
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
        uint16_t newPC = m->Read16(0x0100 + S);
        PC = newPC;
        S += 2;
        cycles = 6;
        break;
    }
    case 0x61:
        break;
    case 0x65:
        break;
    case 0x66:
        break;
    case 0x68:
        break;
    case 0x69:
        break;
    case 0x6a:
        break;
    case 0x6c:
        break;
    case 0x6d:
        break;
    case 0x6e:
        break;
    case 0x70: // BVS
    {
        int8_t offset = m->Read8(PC);
        PC++;
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
    case 0x81:
        break;
    case 0x84:
        break;
    case 0x85: // STA zeropage
    {
        uint16_t addr = m->Read8(PC);
        PC++;
        m->Write8(addr, A);
        cycles = 3;
        break;
    }
    case 0x86:
        break;
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
        int8_t offset = m->Read8(PC);
        PC++;
        cycles = 2;
        if(P.C == 0)
        {
            cycles += 1;
            PC = PC + offset; // TODO: check crosspage
        }
        break;
    }
    case 0x91:
        break;
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
    case 0xa4:
        break;
    case 0xa5: // LDA zeropage
    {
        uint16_t addr = m->Read8(PC);
        PC++;
        A = m->Read8(addr);
        if(A == 0)
            P.Z = 1;
        P.N = (A & 128) ? 1 : 0;
        cycles = 3;
        break;
    }
    case 0xa6:
        break;
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
        A = m->Read8(PC);
        PC++;
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
    case 0xac:
        break;
    case 0xad:
        break;
    case 0xae:
        break;
    case 0xb0: // BCS
    {
        int8_t offset = m->Read8(PC);
        PC++;
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
        uint16_t addr = m->Read8(PC);
        PC++;
        int8_t arg = m->Read8(addr);
        if((int8_t)A >= arg)
        {
            P.C = 1;
        }
        else if((int8_t)A == arg)
        {
            P.Z = 1;
        }
        else if((int8_t)A < arg)
        {
            P.N = 1;
        }
        cycles = 3;
        break;
    }
    case 0xc6:
        break;
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
        int8_t arg = m->Read8(PC);
        PC++;
        if((int8_t)A >= arg)
        {
            P.C = 1;
        }
        else if((int8_t)A == arg)
        {
            P.Z = 1;
        }
        else if((int8_t)A < arg)
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
    case 0xce:
        break;
    case 0xd0: // BNE
    {
        int8_t offset = m->Read8(PC);
        PC++;
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
    case 0xe6:
        break;
    case 0xe8: // INX
    {
        X++;
        if(X == 0)
            P.Z = 1;
        P.N = (X & 128) ? 1 : 0;
        cycles = 2;
        break;
    }
    case 0xe9:
        break;
    case 0xea: // NOP
    {
        cycles = 2;
        break;
    }
    case 0xec:
        break;
    case 0xed:
        break;
    case 0xee:
        break;
    case 0xf0: // BEQ
    {
        int8_t offset = m->Read8(PC);
        PC++;
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
    case 0xf6:
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
        break; // unknown opcode
    }

    return cycles;
}

CPU::~CPU()
{

}