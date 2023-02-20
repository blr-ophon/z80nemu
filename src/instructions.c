#include <assert.h>
#include "cpu.h"
//TODO: fix function head parameter names. reg_x is confusing now

void instruction_res_set_IXIY(struct cpu8080 *cpu, uint8_t opcode, bool bit_state, uint8_t *ixy_operand){
    //instruction format: 00bbbrrr, to set/reset bit b of (ix/y +d) and store in register r 
    uint8_t temp = *ixy_operand;
    uint8_t bit_pos = (opcode & 0x38) >> 3; 
    if(bit_state){
        temp |= (uint8_t) 0x01 << bit_pos;
    }else{
        temp &= ~((uint8_t) 0x01 << bit_pos);
    }

    uint8_t reg = opcode & 0x07; 
    uint8_t *regsPtrs[] = {
        &cpu->reg_B,
        &cpu->reg_C,
        &cpu->reg_D,
        &cpu->reg_E,
        &cpu->reg_H,
        &cpu->reg_L,
        ixy_operand,
        &cpu->reg_A
    };
    *(regsPtrs[reg]) = temp;
}

void instruction_bit_IXIY(struct cpu8080 *cpu, uint8_t opcode, uint8_t *ixy_operand){
    //instruction format: 00bbbrrr, to test bit b of register r. In this case,
    //r is always (IX/Y + d), given in reg_x 
    uint8_t tested_reg = *ixy_operand;

    uint8_t bit_pos = (opcode & 0x38) >> 3; 
    if((tested_reg >> bit_pos) & 0x01){
        cpu->flags.z = 0;
    } else cpu->flags.z = 1;
    cpu->flags.n = 0;
    cpu->flags.h = 1;
}

void instruction_ld_IXIY(struct cpu8080 *cpu, uint8_t opcode, bool iy_mode){
    //instruction format: 01RRRrrr, store contents of r to R 
    uint8_t reg1 = (opcode & 0x38) >> 3;
    uint8_t reg2 = opcode & 0x07;
    uint16_t adr = 0;

    //create a copy of IX or IY in separate 8 bit registers
    uint16_t *ix_or_iy = iy_mode? &cpu->reg_IY : &cpu->reg_IX;
    uint8_t reg_IXYH = ((*ix_or_iy) & 0xff00) >> 8;
    uint8_t reg_IXYL = (*ix_or_iy);

    uint8_t *regsPtrs[] = {
        &cpu->reg_B,
        &cpu->reg_C,
        &cpu->reg_D,
        &cpu->reg_E,
        &reg_IXYH,
        &reg_IXYL,
        NULL, //no need to fetch this when (ix/y + d) is not an operand
        &cpu->reg_A
    };

    //(ix/y + d) is an operand. ixyh and ixyl become H and L
    if(reg1 == 0x6 || reg2 == 0x6){ 
        adr = (*ix_or_iy) + (int16_t) memory_read8(cpu->memory, ++cpu->PC);
        regsPtrs[6] = &cpu->memory->memory[adr];
        regsPtrs[5] = &cpu->reg_L;
        regsPtrs[4] = &cpu->reg_H;
    }

    //LD (*targetPtr),(*sourcePtr) 
    uint8_t *sourcePtr = regsPtrs[reg2];
    uint8_t *targetPtr = regsPtrs[reg1];
    *targetPtr = *sourcePtr;

    //load modified copy back to IX or IY register
    (*ix_or_iy) = ((uint16_t)(reg_IXYH) << 8) | (reg_IXYL);
}

void instruction_ld(struct cpu8080 *cpu, uint8_t opcode){
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

void instruction_sla(struct cpu8080 *cpu, uint8_t *reg_x){
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

void instruction_sll(struct cpu8080 *cpu, uint8_t *reg_x){
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

void instruction_sra(struct cpu8080 *cpu, uint8_t *reg_x){
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

void instruction_srl(struct cpu8080 *cpu, uint8_t *reg_x){
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

void instruction_rl(struct cpu8080 *cpu, uint8_t *reg_x){
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

void instruction_rr(struct cpu8080 *cpu, uint8_t *reg_x){
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

void instruction_rlc(struct cpu8080 *cpu, uint8_t *reg_x){
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

void instruction_rrc(struct cpu8080 *cpu, uint8_t *reg_x){
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

void instruction_inc(struct cpu8080 *cpu, uint8_t *reg_x){
    uint8_t result = *reg_x + 1;
    flags_test_ZS(&cpu->flags, result);
    flags_test_H(&cpu->flags, *reg_x, 1, 0);
    flags_test_V(&cpu->flags, *reg_x, 1, 0);
    cpu->flags.n = 0;
    *reg_x = result;
}

void instruction_dec(struct cpu8080 *cpu, uint8_t *reg_x){
    uint8_t result = *reg_x + ~1 + 1;
    flags_test_ZS(&cpu->flags, result);
    flags_test_H(&cpu->flags, *reg_x, ~1, 1);
    cpu->flags.h = !cpu->flags.h;
    flags_test_V(&cpu->flags, *reg_x, ~1, 1);
    cpu->flags.n = 1;
    *reg_x = result;
}

void instruction_add(struct cpu8080 *cpu, uint8_t reg_x){
    uint16_t result = cpu->reg_A + reg_x;
    flags_test_ZS(&cpu->flags, result);
    flags_test_H(&cpu->flags, cpu->reg_A, reg_x, 0);
    flags_test_V(&cpu->flags, cpu->reg_A, reg_x, 0);
    flags_test_C8(&cpu->flags, result);
    cpu->flags.n = 0;

    cpu->reg_A = result;
}


void instruction_adc(struct cpu8080 *cpu, uint8_t reg_x){
    uint16_t result = cpu->reg_A + reg_x + cpu->flags.cy;
    flags_test_ZS(&cpu->flags, result);
    flags_test_H(&cpu->flags, cpu->reg_A, reg_x, cpu->flags.cy);
    flags_test_V(&cpu->flags, cpu->reg_A, reg_x, cpu->flags.cy);
    flags_test_C8(&cpu->flags, result);
    cpu->flags.n = 0;

    cpu->reg_A = result;
}

void instruction_sub(struct cpu8080 *cpu, uint8_t reg_x){
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


void instruction_sbc(struct cpu8080 *cpu, uint8_t reg_x){
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

void instruction_cmp(struct cpu8080 *cpu, uint8_t reg_x){
    uint8_t temp = cpu->reg_A;
    instruction_sub(cpu, reg_x);
    cpu->reg_A = temp;
}

void instruction_ana(struct cpu8080 *cpu, uint8_t reg_x){
    cpu->reg_A &= reg_x;
    flags_test_ZS(&cpu->flags, cpu->reg_A);
    cpu->flags.h = 1;
    flags_test_P(&cpu->flags, cpu->reg_A);
    cpu->flags.n = 0;
    cpu->flags.cy = 0;
}

void instruction_xra(struct cpu8080 *cpu, uint8_t reg_x){
    cpu->reg_A ^= reg_x;
    flags_test_ZS(&cpu->flags, cpu->reg_A);
    cpu->flags.h = 0;
    flags_test_P(&cpu->flags, cpu->reg_A);
    cpu->flags.n = 0;
    cpu->flags.cy = 0;
}

void instruction_ora(struct cpu8080 *cpu, uint8_t reg_x){
    cpu->reg_A |= reg_x;
    flags_test_ZS(&cpu->flags, cpu->reg_A);
    cpu->flags.h = 0;
    flags_test_P(&cpu->flags, cpu->reg_A);
    cpu->flags.n = 0;
    cpu->flags.cy = 0;
}



