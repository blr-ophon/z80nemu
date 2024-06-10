#include "instr_ixybit.h"

void instr_ixybit(Cpuz80 *cpu, uint8_t opcode, uint8_t Imm, bool iy){
    uint16_t *pIXY = iy? &cpu->reg_IY : &cpu->reg_IX;

    uint8_t opcode_xx = (opcode & 0xc0) >> 6;     //1100 0000    
    uint8_t opcode_yyy = (opcode & 0x38) >> 3;     //0011 1000
    uint8_t opcode_zzz = opcode & 0x07;                   //0000 0111

    //adr = ixy+d
    uint16_t adr = (*pIXY) + (int16_t) Imm;
    
    uint8_t *regsPtrs[] = {
        &cpu->reg_B,
        &cpu->reg_C,
        &cpu->reg_D,
        &cpu->reg_E,
        &cpu->reg_H,
        &cpu->reg_L,
        &cpu->memory->memory[adr],      //(ix + d)
        &cpu->reg_A
    };
    void(*operations_rs[])(Cpuz80 *cpu, uint8_t *reg_x) = {
        instr_bit_RLC,
        instr_bit_RRC,
        instr_bit_RL,
        instr_bit_RR,
        instr_bit_SLA,
        instr_bit_SRA,
        instr_bit_SLL,
        instr_bit_SRL,
    };

    uint8_t tmpReg = *regsPtrs[6];
    switch(opcode_xx){
        case 0: //Rotate, Shift
            operations_rs[opcode_yyy](cpu, &tmpReg);
            *regsPtrs[opcode_zzz] = tmpReg;
            break;
        case 1: //Bit
            instr_bit_BIT(cpu, opcode_yyy, regsPtrs[6]);
            break;
        case 2: //Reset
            instr_bit_RES_SET(cpu, opcode_yyy, &tmpReg, 0);
            *regsPtrs[opcode_zzz] = tmpReg;
            break;
        case 3: //Set
            instr_bit_RES_SET(cpu, opcode_yyy, &tmpReg, 1);
            *regsPtrs[opcode_zzz] = tmpReg;
            break;
    }
}
