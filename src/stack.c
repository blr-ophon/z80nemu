#include "stack.h"

void stack_push16(Cpuz80 *cpu, uint16_t value){
    cpu->memory->memory[--cpu->SP] = (value >> 8); 
    cpu->memory->memory[--cpu->SP] = (value & 0x00ff); 
}

uint16_t stack_pop16(Cpuz80 *cpu){
    uint16_t value = 0x0000; 
    value ^= cpu->memory->memory[cpu->SP++];
    value ^= (uint16_t) cpu->memory->memory[cpu->SP++] << 8;
    return value;
}

void stack_push8(Cpuz80 *cpu, uint8_t value){
    cpu->memory->memory[--cpu->SP] = value; 
}

uint8_t stack_pop8(Cpuz80 *cpu){
    uint8_t value;
    value = cpu->memory->memory[cpu->SP++];
    return value;
}
