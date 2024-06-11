#include "instructions.h"

//TODO: fix function head parameter names. reg_x is confusing now


void instruction_ld(Cpuz80 *cpu, uint8_t opcode){
    //instruction format: 01RRRrrr, store contents of r to R 
    //
    //IMPORTANT: Doesnt work with opcode 0x76, which would be LD (HL),(HL),
    //but instead is HALT
    //
    uint8_t reg1 = (opcode & 0x38) >> 3;
    uint8_t reg2 = opcode & 0x07;

    uint16_t adr = 0;
    if(reg1 == 6 || reg2 == 6){
        adr = read_reg_HL(cpu);
    }

    uint8_t *regs[] = {
        &cpu->reg_B,
        &cpu->reg_C,
        &cpu->reg_D,
        &cpu->reg_E,
        &cpu->reg_H,
        &cpu->reg_L,
        &cpu->memory->memory[adr],
        &cpu->reg_A
    };

    uint8_t *sourcePtr = regs[reg2];
    uint8_t *targetPtr = regs[reg1];
    *targetPtr = *sourcePtr;
}

void instruction_res_set(Cpuz80 *cpu, uint8_t opcode, bool bit_state){
    //instruction format: 00bbbrrr, to set/reset bit b of register r 
    uint8_t bit_pos = (opcode & 0x38) >> 3; 
    uint8_t reg = opcode & 0x07; 
    uint8_t *regPtr;
    switch(reg){
        case 0x0: //B
            regPtr = &cpu->reg_B;
            break;
        case 0x1: //C
            regPtr = &cpu->reg_C;
            break;
        case 0x2: //D
            regPtr = &cpu->reg_D;
            break;
        case 0x3: //E
            regPtr = &cpu->reg_E;
            break;
        case 0x4: //H
            regPtr = &cpu->reg_H;
            break;
        case 0x5: //L
            regPtr = &cpu->reg_L;
            break;
        case 0x6: //(HL)
            regPtr = &PTR_HL;
            break;
        case 0x7: //A
            regPtr = &cpu->reg_A;
            break;
    }
    if(bit_state){
        *regPtr |= (uint8_t) 0x01 << bit_pos;
    }else{
        *regPtr &= ~((uint8_t) 0x01 << bit_pos);
    }
}

void instruction_bit(Cpuz80 *cpu, uint8_t opcode){
    //instruction format: 00bbbrrr, to test bit b of register r 
    uint8_t bit_pos = (opcode & 0x38) >> 3; 
    uint8_t reg = opcode & 0x07; 
    uint8_t temp = 0;
    switch(reg){
        case 0x0: //B
            temp = cpu->reg_B;
            break;
        case 0x1: //C
            temp = cpu->reg_C;
            break;
        case 0x2: //D
            temp = cpu->reg_D;
            break;
        case 0x3: //E
            temp = cpu->reg_E;
            break;
        case 0x4: //H
            temp = cpu->reg_H;
            break;
        case 0x5: //L
            temp = cpu->reg_L;
            break;
        case 0x6: //(HL)
            temp = PTR_HL;
            break;
        case 0x7: //A
            temp = cpu->reg_A;
            break;
    }
    if((temp >> bit_pos) & 0x01){
        cpu->flags.z = 0;
    } else cpu->flags.z = 1;
    cpu->flags.n = 0;
    cpu->flags.h = 1;
}

void instruction_sla(Cpuz80 *cpu, uint8_t *reg_x){
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

void instruction_sll(Cpuz80 *cpu, uint8_t *reg_x){
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

void instruction_sra(Cpuz80 *cpu, uint8_t *reg_x){
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

void instruction_srl(Cpuz80 *cpu, uint8_t *reg_x){
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

void instruction_rl(Cpuz80 *cpu, uint8_t *reg_x){
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

void instruction_rr(Cpuz80 *cpu, uint8_t *reg_x){
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

void instruction_rlc(Cpuz80 *cpu, uint8_t *reg_x){
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

void instruction_rrc(Cpuz80 *cpu, uint8_t *reg_x){
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

void instruction_inc(Cpuz80 *cpu, uint8_t *reg_x){
    uint8_t result = *reg_x + 1;
    flags_test_ZS(&cpu->flags, result);
    flags_test_H(&cpu->flags, *reg_x, 1, 0);
    flags_test_V(&cpu->flags, *reg_x, 1, 0);
    cpu->flags.n = 0;
    *reg_x = result;
}

void instruction_dec(Cpuz80 *cpu, uint8_t *reg_x){
    uint8_t result = *reg_x + ~1 + 1;
    flags_test_ZS(&cpu->flags, result);
    flags_test_H(&cpu->flags, *reg_x, ~1, 1);
    cpu->flags.h = !cpu->flags.h;
    flags_test_V(&cpu->flags, *reg_x, ~1, 1);
    cpu->flags.n = 1;
    *reg_x = result;
}

void instruction_add(Cpuz80 *cpu, uint8_t reg_x){
    uint16_t result = cpu->reg_A + reg_x;
    flags_test_ZS(&cpu->flags, result);
    flags_test_H(&cpu->flags, cpu->reg_A, reg_x, 0);
    flags_test_V(&cpu->flags, cpu->reg_A, reg_x, 0);
    flags_test_C8(&cpu->flags, result);
    cpu->flags.n = 0;

    cpu->reg_A = result;
}


void instruction_adc(Cpuz80 *cpu, uint8_t reg_x){
    uint16_t result = cpu->reg_A + reg_x + cpu->flags.cy;
    flags_test_ZS(&cpu->flags, result);
    flags_test_H(&cpu->flags, cpu->reg_A, reg_x, cpu->flags.cy);
    flags_test_V(&cpu->flags, cpu->reg_A, reg_x, cpu->flags.cy);
    flags_test_C8(&cpu->flags, result);
    cpu->flags.n = 0;

    cpu->reg_A = result;
}

void instruction_sub(Cpuz80 *cpu, uint8_t reg_x){
    uint16_t result = cpu->reg_A + (uint16_t)~(reg_x) + 1;
    flags_test_ZS(&cpu->flags, result);
    flags_test_H(&cpu->flags, cpu->reg_A, ~reg_x, 1);
    cpu->flags.h = !cpu->flags.h;
    flags_test_V(&cpu->flags, cpu->reg_A, ~reg_x, 1);

    cpu->flags.cy = 1;
    if((result ^ cpu->reg_A ^ ~reg_x) & 0x0100){ 
        cpu->flags.cy = 0;
    }
    cpu->flags.n = 1;

    cpu->reg_A = result;
}


void instruction_sbc(Cpuz80 *cpu, uint8_t reg_x){
    uint8_t borrow = ~(cpu->flags.cy) & 0x01;
    uint16_t result = cpu->reg_A + (uint16_t)~(reg_x) + borrow;
    flags_test_ZS(&cpu->flags, result);
    flags_test_H(&cpu->flags, cpu->reg_A, ~reg_x, borrow);
    cpu->flags.h = !cpu->flags.h;
    flags_test_V(&cpu->flags, cpu->reg_A, ~(reg_x), borrow);

    cpu->flags.cy = 1;
    if((result ^ cpu->reg_A ^ ~reg_x) & 0x0100){ 
        cpu->flags.cy = 0;
    }
    cpu->flags.n = 1;
    
    cpu->reg_A = result;
}

void instruction_cmp(Cpuz80 *cpu, uint8_t reg_x){
    uint8_t temp = cpu->reg_A;
    instruction_sub(cpu, reg_x);
    cpu->reg_A = temp;
}

void instruction_ana(Cpuz80 *cpu, uint8_t reg_x){
    cpu->reg_A &= reg_x;
    flags_test_ZS(&cpu->flags, cpu->reg_A);
    cpu->flags.h = 1;
    flags_test_P(&cpu->flags, cpu->reg_A);
    cpu->flags.n = 0;
    cpu->flags.cy = 0;
}

void instruction_xra(Cpuz80 *cpu, uint8_t reg_x){
    cpu->reg_A ^= reg_x;
    flags_test_ZS(&cpu->flags, cpu->reg_A);
    cpu->flags.h = 0;
    flags_test_P(&cpu->flags, cpu->reg_A);
    cpu->flags.n = 0;
    cpu->flags.cy = 0;
}

void instruction_ora(Cpuz80 *cpu, uint8_t reg_x){
    cpu->reg_A |= reg_x;
    flags_test_ZS(&cpu->flags, cpu->reg_A);
    cpu->flags.h = 0;
    flags_test_P(&cpu->flags, cpu->reg_A);
    cpu->flags.n = 0;
    cpu->flags.cy = 0;
}



