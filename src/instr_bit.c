#include "instr_bit.h"

//TODO: Document functions

/*
 * single byte instructions, no immediates
 * (xx)(yyy)(zzz)
 * xx:
 *   00: Shift/Rotate reg
 *   01: Bit n, reg
 *   10: Res n, reg
 *   11: Set n, reg
 * yyy:
 *   type of shift/rotate or n
 * zzz:
 *   reg
 */
void instr_bit(Cpuz80 *cpu, uint8_t opcode){
    uint8_t opcode_xx = (opcode & 0xc0) >> 6;     //1100 0000    
    uint8_t opcode_yyy = (opcode & 0x38) >> 3;     //0011 1000
    uint8_t opcode_zzz = opcode & 0x07;                   //0000 0111

    uint8_t *regsPtrs[] = {
        &cpu->reg_B,
        &cpu->reg_C,
        &cpu->reg_D,
        &cpu->reg_E,
        &cpu->reg_H,
        &cpu->reg_L,
        &cpu->memory->memory[read_reg_HL(cpu)],
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

    switch(opcode_xx){
        case 0: //Rotate, Shift
            operations_rs[opcode_yyy](cpu, regsPtrs[opcode_zzz]);
            break;
        case 1: //Bit
            instr_bit_BIT(cpu, opcode_yyy, regsPtrs[opcode_zzz]);
            break;
        case 2: //Reset
            instr_bit_RES_SET(cpu, opcode_yyy, regsPtrs[opcode_zzz], 0);
            break;
        case 3: //Set
            instr_bit_RES_SET(cpu, opcode_yyy, regsPtrs[opcode_zzz], 1);
            break;
    }
}

/*
 * Bit n, reg_x
 * Test bit n of reg_x
 */
void instr_bit_BIT(Cpuz80 *cpu, uint8_t n, uint8_t *reg_x){
    if((*reg_x >> n) & 0x01){
        cpu->flags.z = 0;
    } else cpu->flags.z = 1;
    cpu->flags.n = 0;
    cpu->flags.h = 1;
}


/*
 * RES/SET n, reg_x
 * Resets/Sets bit n of reg_x
 * res_set: 0 for res, 1 for set
 */
void instr_bit_RES_SET(Cpuz80 *cpu, uint8_t n, uint8_t *reg_x, bool res_set){
    if(res_set){ //SET
        *reg_x |= (uint8_t) 0x01 << n;

    }else{ //RESET
        *reg_x &= ~((uint8_t) 0x01 << n);
    }
}




void instr_bit_RLC(Cpuz80 *cpu, uint8_t *reg_x){
    instruction_rlc(cpu, reg_x);
    flags_test_ZS(&cpu->flags, *reg_x);
    flags_test_P(&cpu->flags, *reg_x);
}

void instr_bit_RRC(Cpuz80 *cpu, uint8_t *reg_x){
    instruction_rrc(cpu, reg_x);
    flags_test_ZS(&cpu->flags, *reg_x);
    flags_test_P(&cpu->flags, *reg_x);
}

void instr_bit_RL(Cpuz80 *cpu, uint8_t *reg_x){
    instruction_rl(cpu, reg_x);
    flags_test_ZS(&cpu->flags, *reg_x);
    flags_test_P(&cpu->flags, *reg_x);
}

void instr_bit_RR(Cpuz80 *cpu, uint8_t *reg_x){
    instruction_rr(cpu, reg_x);
    flags_test_ZS(&cpu->flags, *reg_x);
    flags_test_P(&cpu->flags, *reg_x);
}

void instr_bit_SLA(Cpuz80 *cpu, uint8_t *reg_x){
    instruction_sla(cpu, reg_x);
}

void instr_bit_SRA(Cpuz80 *cpu, uint8_t *reg_x){
    instruction_sra(cpu, reg_x);
}

void instr_bit_SLL(Cpuz80 *cpu, uint8_t *reg_x){
    instruction_sll(cpu, reg_x);
}


void instr_bit_SRL(Cpuz80 *cpu, uint8_t *reg_x){
    instruction_srl(cpu, reg_x);
}

