#include "registerbank.h"
#include "cpu.h"


void write_reg_BC(Cpuz80 *cpu, uint16_t value){
    cpu->reg_B = (value & 0xff00) >> 8;
    cpu->reg_C = value & 0x00ff;
}

uint16_t read_reg_BC(Cpuz80 *cpu){
    uint16_t word = cpu->reg_C;
    word ^= (uint16_t) cpu->reg_B << 8;
    return word;
}

void write_reg_DE(Cpuz80 *cpu, uint16_t value){
    cpu->reg_D = (value & 0xff00) >> 8;
    cpu->reg_E = value & 0x00ff;
}

uint16_t read_reg_DE(Cpuz80 *cpu){
    uint16_t word = cpu->reg_E;
    word ^= (uint16_t) cpu->reg_D << 8;
    return word;
}

void write_reg_HL(Cpuz80 *cpu, uint16_t value){
    cpu->reg_H = (value & 0xff00) >> 8;
    cpu->reg_L = value & 0x00ff;
}

uint16_t read_reg_HL(Cpuz80 *cpu){
    uint16_t word = cpu->reg_L;
    word ^= (uint16_t) cpu->reg_H << 8;
    return word;
}

void write_reg_SP(Cpuz80 *cpu, uint16_t value){
    cpu->SP = value;
}

uint16_t read_reg_SP(Cpuz80 *cpu){
    return cpu->SP;
}

void write_reg_AF(Cpuz80 *cpu, uint16_t value){
    cpu->reg_A = value >> 8;
    flags_sta_byte(&cpu->flags, (uint8_t)value);
}

uint16_t read_reg_AF(Cpuz80 *cpu){
    uint16_t word = 0;
    word |= (uint16_t) (cpu->reg_A)  << 8;
    word |= flags_load_byte(&cpu->flags);
    return word;
}
