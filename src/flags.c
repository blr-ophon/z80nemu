#include "flags.h"

void flags_test_V(Flags *flags, uint8_t a, uint8_t b){
    //TODO: logic for a+b+carry 
    if((a^b) & 0x80){ //if a and b have different sign bits
        flags->p = 0;
    }
    else{
        uint8_t result = a + b;
        //compares sign of initial value and result
        uint8_t diff_sign = (a^result) & 0x80;
        //if the signs different, set overflow
        flags->p = diff_sign? 1 : 0;
    }
}

void flags_test_V16(Flags *flags, uint16_t a, uint16_t b){
    //TODO: logic for a+b+carry 
    if((a^b) & 0x8000){ //if a and b have different sign bits
        flags->p = 0;
    }
    else{
        uint16_t result = a + b;
        //compares sign of initial value and result
        uint16_t diff_sign = (a^result) & 0x8000;
        //if the signs different, set overflow
        flags->p = diff_sign? 1 : 0;
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
    flags->z = result == 0? 1 : 0;
    flags->s = result & 0x80? 1 : 0;
}

void flags_test_H(Flags *flags, uint8_t value, uint8_t operand, uint8_t carry){
    //tests the overflow of bit 3
    value &=  0x0f;
    operand &=  0x0f;
    carry &= 0x01;
    
    uint8_t result = value + operand + carry;
    uint8_t changes = result ^ value ^ operand;

    flags->h = changes > 0x0f? 1 : 0;
}

void flags_test_H16(Flags *flags, uint16_t value, uint16_t operand, uint8_t carry){
    //tests the overflow of bit 11
    value &=  0x00ff;
    operand &=  0x00ff;
    carry &= 0x01;
    
    uint16_t result = value + operand + carry;
    uint16_t changes = result ^ value ^ operand;

    flags->h = changes > 0x0fff? 1 : 0;
}

void flags_test_C8(Flags *flags, uint16_t result){
    flags->cy = result & 0x100? 1 : 0;
}

void flags_test_C16(Flags *flags, uint32_t result){
    flags->cy = result & 0x10000? 1 : 0;
}

uint8_t flags_load_byte(Flags *flags){
    uint8_t flags_reg = 0;
    flags_reg |= flags->s << 7;     //10000000
    flags_reg |= flags->z << 6;     //01000000
    flags_reg |= flags->h << 4;    //00010000
    flags_reg |= flags->p << 2;     //00000100
    flags_reg |= flags->n << 1;     //00000010
    flags_reg |= flags->cy;         //00000001        
    return flags_reg;
}

void flags_sta_byte(Flags *flags, uint8_t flags_reg){
    flags->s = (flags_reg &  0x80) >> 7;
    flags->z = (flags_reg &  0x40) >> 6;
    flags->h = (flags_reg & 0x10) >> 4;
    flags->p = (flags_reg &  0x04) >> 2;
    flags->n = (flags_reg &  0x02) >> 1;
    flags->cy = flags_reg &  0x01;
}
