#include "registerbank.h"
#include "cpu.h"


void write_reg_BC(Cpu8080 *cpu, uint16_t value){
    cpu->reg_B = (value & 0xff00) >> 8;
    cpu->reg_C = value & 0x00ff;
}

uint16_t read_reg_BC(Cpu8080 *cpu){
    uint16_t word = cpu->reg_C;
    word ^= (uint16_t) cpu->reg_B << 8;
    return word;
}

void write_reg_DE(Cpu8080 *cpu, uint16_t value){
    cpu->reg_D = (value & 0xff00) >> 8;
    cpu->reg_E = value & 0x00ff;
}

uint16_t read_reg_DE(Cpu8080 *cpu){
    uint16_t word = cpu->reg_E;
    word ^= (uint16_t) cpu->reg_D << 8;
    return word;
}

void write_reg_HL(Cpu8080 *cpu, uint16_t value){
    cpu->reg_H = (value & 0xff00) >> 8;
    cpu->reg_L = value & 0x00ff;
}

uint16_t read_reg_HL(Cpu8080 *cpu){
    uint16_t word = cpu->reg_L;
    word ^= (uint16_t) cpu->reg_H << 8;
    return word;
}
