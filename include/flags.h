#ifndef FLAGS_H
#define FLAGS_H
#include <stdint.h>
#include <stdbool.h>

typedef struct flags{
    //TODO: change flag names to the z80 pattern
    uint8_t s:1;  //sign
    uint8_t z:1;  //zero 
    uint8_t ac:1; //auxiliary carry
    uint8_t p:1;  //parity/overflow
    uint8_t n:1;  //add/subtract
    uint8_t cy:1; //carry
}Flags;

void flags_test_V(Flags *flags, uint8_t a, uint8_t b);

void flags_test_P(Flags *flags, uint8_t result);

void flags_test_ZS(Flags *flags, uint8_t result);

void flags_test_C8(Flags *flags, uint16_t result);

void flags_test_C16(Flags *flags, uint32_t result);

void flags_test_H(Flags *flags, uint8_t value, uint8_t operand, uint8_t carry);

void flags_test_H16(Flags *flags, uint16_t value, uint16_t operand, uint8_t carry);

uint8_t flags_load_byte(Flags *flags);

void flags_sta_byte(Flags *flags, uint8_t flags_reg);

#endif
