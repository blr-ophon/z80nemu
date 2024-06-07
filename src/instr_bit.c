#include "instr_bit.h"
#include "cpu.h"

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
void instr_bit(struct cpuz80 *cpu, uint8_t opcode){
    /*
     * Index 1 defines if it is a Shift/Rotate or a bit/res/set instruction
     * Index 2 specifies the kind of shift/rotate or bit/res/set instruction
     * regIndex defines the register used in the operation
     */
    uint8_t operationIndex1 = (opcode & 0xc0) >> 6;     //1100 0000    
    uint8_t operationIndex2 = (opcode & 0x38) >> 3;     //0011 1000
    uint8_t regIndex = opcode & 0x07;                   //0000 0111

    uint16_t adr = 0;
    if(1){
        adr = read_reg_HL(cpu);
    }

    uint8_t *regsPtrs[] = {
        &cpu->reg_B,
        &cpu->reg_C,
        &cpu->reg_D,
        &cpu->reg_E,
        &cpu->reg_H,
        &cpu->reg_L,
        &cpu->memory->memory[adr],
        &cpu->reg_A
    };

    void(*operations_rs[])(struct cpuz80 *cpu, uint8_t *reg_x) = {
        instr_bit_RLC,
        instr_bit_RRC,
        instr_bit_RL,
        instr_bit_RR,
        instr_bit_SLA,
        instr_bit_SRA,
        instr_bit_SLL,
        instr_bit_SRL,
    };

    switch(operationIndex1){
        case 0: //Rotate, Shift
            operations_rs[operationIndex2](cpu, regsPtrs[regIndex]);
            break;
        case 1: //Bit
            instr_bit_BIT(cpu, operationIndex2, regsPtrs[regIndex]);
            break;
        case 2: //Reset
            instr_bit_RES_SET(cpu, operationIndex2, regsPtrs[regIndex], 0);
            break;
        case 3: //Set
            instr_bit_RES_SET(cpu, operationIndex2, regsPtrs[regIndex], 1);
            break;
        default:
            //TODO: return error
    }
}

/*
 * Bit n, reg_x
 * Test bit n of reg_x
 */
void instr_bit_BIT(struct cpuz80 *cpu, uint8_t n, uint8_t *reg_x){
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
void instr_bit_RES_SET(struct cpuz80 *cpu, uint8_t n, uint8_t *reg_x, bool res_set){
    if(res_set){ //SET
        *reg_x |= (uint8_t) 0x01 << n;

    }else{ //RESET
        *reg_x &= ~((uint8_t) 0x01 << n);
    }
}




void instr_bit_RLC(struct cpuz80 *cpu, uint8_t *reg_x){
    uint8_t bit7 = *reg_x & 0x80;
    *reg_x <<= 1;
    if(bit7){
        cpu->flags.cy = 1;
        *reg_x |= 0x01;
    }else{
        cpu->flags.cy = 0;
        *reg_x &= ~0x01;
    }
    cpu->flags.n = 0;
    cpu->flags.h = 0;
}

void instr_bit_RRC(struct cpuz80 *cpu, uint8_t *reg_x){
    uint8_t bit0 = *reg_x & 0x01;
    *reg_x >>= 1;
    if(bit0){
        cpu->flags.cy = 1;
        *reg_x |= 0x80;
    }else{
        cpu->flags.cy = 0;
        *reg_x &= ~0x80;
    }
    cpu->flags.n = 0;
    cpu->flags.h = 0;
}

void instr_bit_RL(struct cpuz80 *cpu, uint8_t *reg_x){
    uint8_t bit7 = *reg_x & 0x80;
    *reg_x <<= 1;
    if(cpu->flags.cy){
        *reg_x |= 0x01;
    }else{
        *reg_x &= ~0x01;
    }

    if(bit7){
        cpu->flags.cy = 1;
    }else{
        cpu->flags.cy = 0;
    }
    cpu->flags.n = 0;
    cpu->flags.h = 0;
}

void instr_bit_RR(struct cpuz80 *cpu, uint8_t *reg_x){
    uint8_t bit0 = *reg_x & 0x01;
    *reg_x >>= 1;
    if(cpu->flags.cy){
        *reg_x |= 0x80;
    }else{
        *reg_x &= ~0x80;
    }

    if(bit0){
        cpu->flags.cy = 1;
    }else{
        cpu->flags.cy = 0;
    }
    cpu->flags.n = 0;
    cpu->flags.h = 0;
}

void instr_bit_SLA(struct cpuz80 *cpu, uint8_t *reg_x){
    uint8_t bit7 = *reg_x & 0x80;
    *reg_x <<= 1;

    if(bit7){
        cpu->flags.cy = 1;
    }else{
        cpu->flags.cy = 0;
    }
    cpu->flags.n = 0;
    cpu->flags.h = 0;
    flags_test_ZS(&cpu->flags, *reg_x);
    flags_test_P(&cpu->flags, *reg_x);
}

void instr_bit_SRA(struct cpuz80 *cpu, uint8_t *reg_x){
    //convert to signed to shift right arithmetically
    uint8_t bit7 = *reg_x & 0x80;
    uint8_t bit0 = *reg_x & 0x01;
    *reg_x >>= 1;
    if(bit7) *reg_x |= 0x80;
    else {*reg_x &= ~0x80;}

    if(bit0){
        cpu->flags.cy = 1;
    }else{
        cpu->flags.cy = 0;
    }
    cpu->flags.n = 0;
    cpu->flags.h = 0;
    flags_test_ZS(&cpu->flags, *reg_x);
    flags_test_P(&cpu->flags, *reg_x);
}

void instr_bit_SLL(struct cpuz80 *cpu, uint8_t *reg_x){
    uint8_t bit7 = *reg_x & 0x80;
    *reg_x <<= 1;

    if(bit7){
        cpu->flags.cy = 1;
    }else{
        cpu->flags.cy = 0;
    }
    *reg_x |= 0x01;
    cpu->flags.n = 0;
    cpu->flags.h = 0;
    flags_test_ZS(&cpu->flags, *reg_x);
    flags_test_P(&cpu->flags, *reg_x);
}


void instr_bit_SRL(struct cpuz80 *cpu, uint8_t *reg_x){
    uint8_t bit0 = *reg_x & 0x01;
    *reg_x >>= 1;

    if(bit0){
        cpu->flags.cy = 1;
    }else{
        cpu->flags.cy = 0;
    }
    cpu->flags.n = 0;
    cpu->flags.h = 0;
    flags_test_ZS(&cpu->flags, *reg_x);
    flags_test_P(&cpu->flags, *reg_x);
}

