#ifndef REGISTERBANK_H
#define REGISTERBANK_H
#include <stdint.h>

typedef struct cpu8080 Cpu8080;


void write_reg_BC(Cpu8080 *cpu, uint16_t value);

uint16_t read_reg_BC(Cpu8080 *cpu);

void write_reg_DE(Cpu8080 *cpu, uint16_t value);

uint16_t read_reg_DE(Cpu8080 *cpu);

void write_reg_HL(Cpu8080 *cpu, uint16_t value);

uint16_t read_reg_HL(Cpu8080 *cpu);

#endif
