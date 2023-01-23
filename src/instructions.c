#include "../include/instructions.h"
#include "../include/cpu.h"

void instruction_res_set(struct cpu8080 *cpu, uint8_t opcode, bool bit_state){
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
            regPtr = &PTR(HL);
            break;
        case 0x7: //A
            regPtr = &cpu->reg_A;
            break;
    }
    if(bit_state){
        *regPtr |= (uint8_t) bit_state << bit_pos;
    }else{
        *regPtr &= ~((uint8_t) bit_state << bit_pos);
    }
}

void instruction_bit(struct cpu8080 *cpu, uint8_t opcode){
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
            temp = PTR(HL);
            break;
        case 0x7: //A
            temp = cpu->reg_A;
            break;
    }
    if((temp >> bit_pos) & 0x01){
        cpu->flags.z = 0;
    } else cpu->flags.z = 1;
    cpu->flags.n = 0;
    cpu->flags.ac = 1;
}

void instruction_sla(struct cpu8080 *cpu, uint8_t *reg_x){
    uint8_t bit7 = *reg_x & 0b10000000;
    *reg_x <<= 1;

    if(bit7){
        cpu->flags.cy = 1;
    }else{
        cpu->flags.cy = 0;
    }
    cpu->flags.n = 0;
    cpu->flags.ac = 0;
    test_normal_flags(&cpu->flags, *reg_x);
}

void instruction_sra(struct cpu8080 *cpu, uint8_t *reg_x){
    //convert to signed to shift right arithmetically
    int8_t temp = *reg_x;
    uint8_t bit0 = temp & 0b00000001;
    temp >>= 1;

    if(bit0){
        cpu->flags.cy = 1;
    }else{
        cpu->flags.cy = 0;
    }
    cpu->flags.n = 0;
    cpu->flags.ac = 0;
    test_normal_flags(&cpu->flags, temp);
    *reg_x = temp;
}

void instruction_srl(struct cpu8080 *cpu, uint8_t *reg_x){
    uint8_t bit0 = *reg_x & 0b00000001;
    *reg_x >>= 1;
    //reg_x is unsigned, so right shift is logic

    if(bit0){
        cpu->flags.cy = 1;
    }else{
        cpu->flags.cy = 0;
    }
    cpu->flags.n = 0;
    cpu->flags.ac = 0;
    test_normal_flags(&cpu->flags, *reg_x);
}

void instruction_rl(struct cpu8080 *cpu, uint8_t *reg_x){
    uint8_t bit7 = *reg_x & 0b10000000;
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
    cpu->flags.ac = 0;
}

void instruction_rr(struct cpu8080 *cpu, uint8_t *reg_x){
    uint8_t bit0 = *reg_x & 0b00000001;
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
    cpu->flags.ac = 0;
}

void instruction_rlc(struct cpu8080 *cpu, uint8_t *reg_x){
    uint8_t bit7 = *reg_x & 0b10000000;
    *reg_x <<= 1;
    if(bit7){
        cpu->flags.cy = 1;
        *reg_x |= 0b00000001;
    }else{
        cpu->flags.cy = 0;
        *reg_x &= ~0b00000001;
    }
    cpu->flags.n = 0;
    cpu->flags.ac = 0;
}

void instruction_rrc(struct cpu8080 *cpu, uint8_t *reg_x){
    uint8_t bit0 = *reg_x & 0b00000001;
    *reg_x >>= 1;
    if(bit0){
        cpu->flags.cy = 1;
        *reg_x |= 0b10000000;
    }else{
        cpu->flags.cy = 0;
        *reg_x &= ~0b10000000;
    }
    cpu->flags.n = 0;
    cpu->flags.ac = 0;
}

void instruction_add(struct cpu8080 *cpu, uint8_t reg_x){
    uint16_t result = cpu->reg_A + reg_x;
    test_flag_ac(&cpu->flags, cpu->reg_A, reg_x, 0);
    test_carry_flag8(&cpu->flags, result);

    cpu->reg_A = result;
    test_normal_flags(&cpu->flags, cpu->reg_A);
    cpu->flags.n = 0;
}

void instruction_adc(struct cpu8080 *cpu, uint8_t reg_x){
    uint16_t result = cpu->reg_A + reg_x + cpu->flags.cy;
    test_flag_ac(&cpu->flags, cpu->reg_A, reg_x, cpu->flags.cy);
    test_carry_flag8(&cpu->flags, result);

    cpu->reg_A = result;
    test_normal_flags(&cpu->flags, cpu->reg_A);
    cpu->flags.n = 0;
}

void instruction_sub(struct cpu8080 *cpu, uint8_t reg_x){
    uint16_t result = cpu->reg_A + ~(reg_x) + 1;

    cpu->flags.cy = 0;
    if((result ^ cpu->reg_A ^ ~reg_x) & 0x0100){ 
        cpu->flags.cy = 1;
    }

    test_flag_ac(&cpu->flags, cpu->reg_A, ~reg_x, 1);
    test_normal_flags(&cpu->flags, result);
    cpu->reg_A = result;
    cpu->flags.cy = ~(cpu->flags.cy) & 0x01;
    cpu->flags.n = 1;
}

void instruction_sbb(struct cpu8080 *cpu, uint8_t reg_x){
    uint8_t borrow = ~(cpu->flags.cy) & 0x01;
    uint16_t result = cpu->reg_A + ~reg_x + borrow;

    cpu->flags.cy = 0;
    if((result ^ cpu->reg_A ^ ~reg_x) & 0x0100){ 
        cpu->flags.cy = 1;
    }

    test_flag_ac(&cpu->flags, cpu->reg_A, ~reg_x, borrow);
    test_normal_flags(&cpu->flags, result);
    cpu->reg_A = result;
    cpu->flags.cy = ~cpu->flags.cy & 0x01;
    cpu->flags.n = 1;
}

void instruction_cmp(struct cpu8080 *cpu, uint8_t reg_x){
    uint16_t result = cpu->reg_A - reg_x;
    cpu->flags.cy = result >> 8;
    cpu->flags.ac = 0;
    if(~(cpu->reg_A ^ result ^ reg_x) & 0x10){
        cpu->flags.ac = 1;
    }
    test_normal_flags(&cpu->flags, (uint8_t) result);
    cpu->flags.n = 1;
}

void instruction_ana(struct cpu8080 *cpu, uint8_t reg_x){
    uint8_t result = cpu->reg_A & reg_x;
    cpu->flags.cy = 0;
    cpu->flags.ac = 0;
    cpu->flags.n = 0;
    if(((cpu->reg_A | reg_x) & 0x08) != 0){
        cpu->flags.ac = 1;
    }
    test_normal_flags(&cpu->flags, result);
    cpu->reg_A = result;
}

void instruction_xra(struct cpu8080 *cpu, uint8_t reg_x){
    cpu->reg_A ^= reg_x;
    cpu->flags.ac = 0;
    cpu->flags.cy = 0;
    cpu->flags.n = 0;
    test_normal_flags(&cpu->flags, cpu->reg_A);
}

void instruction_ora(struct cpu8080 *cpu, uint8_t reg_x){
    cpu->reg_A |= reg_x;
    cpu->flags.ac = 0;
    cpu->flags.cy = 0;
    cpu->flags.n = 0;
    test_normal_flags(&cpu->flags, cpu->reg_A);
}



