#include "registerbank.h"
#include "cpu.h"


void write_reg_BC(Cpuz80 *cpu, uint16_t value){
    cpu->reg_B = (value & 0xff00) >> 8;
    cpu->reg_C = value & 0x00ff;
}
void write_reg_DE(Cpuz80 *cpu, uint16_t value){
    cpu->reg_D = (value & 0xff00) >> 8;
    cpu->reg_E = value & 0x00ff;
}
void write_reg_HL(Cpuz80 *cpu, uint16_t value){
    cpu->reg_H = (value & 0xff00) >> 8;
    cpu->reg_L = value & 0x00ff;
}
void write_reg_SP(Cpuz80 *cpu, uint16_t value){
    cpu->SP = value;
}
void write_reg_AF(Cpuz80 *cpu, uint16_t value){
    cpu->reg_A = value >> 8;
    flags_sta_byte(&cpu->flags, (uint8_t)value);
}
void write_reg_IX(Cpuz80 *cpu, uint16_t value){
    cpu->reg_IX = value;
}
void write_reg_IY(Cpuz80 *cpu, uint16_t value){
    cpu->reg_IY = value;
}



uint16_t read_reg_BC(Cpuz80 *cpu){
    uint16_t word = cpu->reg_C;
    word ^= (uint16_t) cpu->reg_B << 8;
    return word;
}
uint16_t read_reg_DE(Cpuz80 *cpu){
    uint16_t word = cpu->reg_E;
    word ^= (uint16_t) cpu->reg_D << 8;
    return word;
}
uint16_t read_reg_HL(Cpuz80 *cpu){
    uint16_t word = cpu->reg_L;
    word ^= (uint16_t) cpu->reg_H << 8;
    return word;
}
uint16_t read_reg_SP(Cpuz80 *cpu){
    return cpu->SP;
}
uint16_t read_reg_AF(Cpuz80 *cpu){
    uint16_t word = 0;
    word |= (uint16_t) (cpu->reg_A)  << 8;
    word |= flags_load_byte(&cpu->flags);
    return word;
}
uint16_t read_reg_IX(Cpuz80 *cpu){
    return cpu->reg_IX;
}
uint16_t read_reg_IY(Cpuz80 *cpu){
    return cpu->reg_IY;
}



void write_reg_B(Cpuz80 *cpu, uint8_t value){
    cpu->reg_B = value;
}
void write_reg_C(Cpuz80 *cpu, uint8_t value){
    cpu->reg_C = value;
}
void write_reg_D(Cpuz80 *cpu, uint8_t value){
    cpu->reg_D = value;
}
void write_reg_E(Cpuz80 *cpu, uint8_t value){
    cpu->reg_E = value;
}
void write_reg_H(Cpuz80 *cpu, uint8_t value){
    cpu->reg_H = value;
}
void write_reg_L(Cpuz80 *cpu, uint8_t value){
    cpu->reg_L = value;
}
void write_reg_pHL(Cpuz80 *cpu, uint8_t value){
    cpu->memory->memory[read_reg_HL(cpu)] = value;
}
void write_reg_A(Cpuz80 *cpu, uint8_t value){
    cpu->reg_A = value;
}



uint8_t read_reg_B(Cpuz80 *cpu){
    return cpu->reg_B;
}
uint8_t read_reg_C(Cpuz80 *cpu){
    return cpu->reg_C;
}
uint8_t read_reg_D(Cpuz80 *cpu){
    return cpu->reg_D;
}
uint8_t read_reg_E(Cpuz80 *cpu){
    return cpu->reg_E;
}
uint8_t read_reg_H(Cpuz80 *cpu){
    return cpu->reg_H;
}
uint8_t read_reg_L(Cpuz80 *cpu){
    return cpu->reg_L;
}
uint8_t read_reg_pHL(Cpuz80 *cpu){
    return cpu->memory->memory[read_reg_HL(cpu)];
}
uint8_t read_reg_A(Cpuz80 *cpu){
    return cpu->reg_L;
}



void write_reg_IXL(Cpuz80 *cpu, uint8_t value){
    *((uint8_t*)&cpu->reg_IX) = value;
}
void write_reg_IXH(Cpuz80 *cpu, uint8_t value){
    *((uint8_t*)&cpu->reg_IX + 1) = value;
}
void write_reg_IYL(Cpuz80 *cpu, uint8_t value){
    *((uint8_t*)&cpu->reg_IY) = value;
}
void write_reg_IYH(Cpuz80 *cpu, uint8_t value){
    *((uint8_t*)&cpu->reg_IY + 1) = value;
}
void write_reg_pIXd(Cpuz80 *cpu, uint8_t value){
    uint16_t addr = cpu->reg_IX + (int8_t)memory_read8(cpu->memory, ++cpu->PC);
    cpu->memory->memory[addr] = value;
}
void write_reg_pIYd(Cpuz80 *cpu, uint8_t value){
    uint16_t addr = cpu->reg_IY + (int8_t)memory_read8(cpu->memory, ++cpu->PC);
    cpu->memory->memory[addr] = value;
}



uint8_t read_reg_IXL(Cpuz80 *cpu){
    return *((uint8_t*)&cpu->reg_IX);
}
uint8_t read_reg_IXH(Cpuz80 *cpu){
    return *((uint8_t*)&cpu->reg_IX + 1);
}
uint8_t read_reg_IYL(Cpuz80 *cpu){
    return *((uint8_t*)&cpu->reg_IY);
}
uint8_t read_reg_IYH(Cpuz80 *cpu){
    return *((uint8_t*)&cpu->reg_IY + 1);
}
uint8_t read_reg_pIXd(Cpuz80 *cpu){
    uint16_t addr = cpu->reg_IX + (int8_t)memory_read8(cpu->memory, ++cpu->PC);
    return cpu->memory->memory[addr];
}
uint8_t read_reg_pIYd(Cpuz80 *cpu){
    uint16_t addr = cpu->reg_IY + (int8_t)memory_read8(cpu->memory, ++cpu->PC);
    return cpu->memory->memory[addr];
}
