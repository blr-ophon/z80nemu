#include "flags.h"

void flags_test_V(Flags *flags, uint8_t a, uint8_t b){
    if((a^b) & 0x80){ //if a and b have different sign bits
        flags->p = 0;
    }
    else{
        uint8_t result = a + b;
        uint8_t diff_sign = (a^result) & 0x80;
        flags->p = diff_sign >> 7;
    }
}

void flags_test_P(Flags *flags, uint8_t result){
    uint8_t parity = 1;
    for(int i = 0; i < 8; i++){
        if((result >> i) & 0x01){
            parity ^= 0x01; //invert bit0
        }
    }
    flags->p = parity;
}

void flags_test_ZS(Flags *flags, uint8_t result){
    //tests z, s p and ac flags
    //
    if(result == 0){
        flags->z = 1;
    }else{
        flags->z = 0;
    }

    if(result & 0x80){
        flags->s = 1;
    }else{
        flags->s = 0;
    }
}

void flags_test_H(Flags *flags, uint8_t value, uint8_t operand, uint8_t carry){
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

void flags_test_C8(Flags *flags, uint16_t result){
    if(result & 0x100){
        flags->cy = 1;
    }else{
        flags->cy = 0;
    }
}

void flags_test_C16(Flags *flags, uint32_t result){
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
    flags_reg |= 0x02;              //00000010
//    flags_reg |= flags->n << 1;     //00000010
    flags_reg |= flags->cy;         //00000001        
    return flags_reg;
}

void flags_sta_byte(Flags *flags, uint8_t flags_reg){
    flags->s = (flags_reg &  0x80) >> 7;
    flags->z = (flags_reg &  0x40) >> 6;
    flags->ac = (flags_reg & 0x10) >> 4;
    flags->p = (flags_reg &  0x04) >> 2;
    flags->n = (flags_reg &  0x02) >> 1;
    flags->cy = flags_reg &  0x01;
}
