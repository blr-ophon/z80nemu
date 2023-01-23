#include "../include/flags.h"

void test_normal_flags(Flags *flags, uint8_t result){
    //tests z, s p and ac flags
    //
    if(result == 0){
        flags->z = 1;
    }else{
        flags->z = 0;
    }

    if(result & 0b10000000){
        flags->s = 1;
    }else{
        flags->s = 0;
    }

    uint8_t parity = 1;
    for(int i = 0; i < 8; i++){
        if((result >> i) & 0x01){
            parity ^= 0x01; //invert bit0
        }
    }
    flags->p = parity;
}

void test_flag_ac(Flags *flags, uint8_t value, uint8_t operand, uint8_t carry){
    //tests the overflow of bit 3
    value &=  0x0f;
    operand &=  0x0f;
    carry &= 0x01;
    
    uint8_t result = value + operand + carry;
    uint8_t changes = result ^ value ^ operand;

    flags->ac = 0;
    if(changes > 0x0f){
        flags->ac = 1;
    }
}

void test_carry_flag8(Flags *flags, uint16_t result){
    if(result & 0x100){
        flags->cy = 1;
    }else{
        flags->cy = 0;
    }
}

void test_carry_flag16(Flags *flags, uint32_t result){
    if(result > 0xffff){
        flags->cy = 1;
    }else{
        flags->cy = 0;
    }
}

uint8_t flags_load_byte(Flags *flags){
    uint8_t flags_reg = 0;
    flags_reg |= flags->s << 7;     //10000000
    flags_reg |= flags->z << 6;     //01000000
    flags_reg |= flags->ac << 4;    //00010000
    flags_reg |= flags->p << 2;     //00000100
    flags_reg |= 0b00000010;     //00000010
//    flags_reg |= flags->n << 1;     //00000010
    flags_reg |= flags->cy;         //00000001        
    return flags_reg;
}

void flags_sta_byte(Flags *flags, uint8_t flags_reg){
    flags->s = (flags_reg &  0b10000000) >> 7;
    flags->z = (flags_reg &  0b01000000) >> 6;
    flags->ac = (flags_reg & 0b00010000) >> 4;
    flags->p = (flags_reg &  0b00000100) >> 2;
    flags->n = (flags_reg &  0b00000010) >> 1;
    flags->cy = flags_reg &  0b00000001;
}
