#ifndef REGISTERBANK_H
#define REGISTERBANK_H
#include <stdint.h>
#include "flags.h"

typedef struct cpuz80 Cpuz80;


void write_reg_BC(Cpuz80 *cpu, uint16_t value);

uint16_t read_reg_BC(Cpuz80 *cpu);

void write_reg_DE(Cpuz80 *cpu, uint16_t value);

uint16_t read_reg_DE(Cpuz80 *cpu);

void write_reg_HL(Cpuz80 *cpu, uint16_t value);

uint16_t read_reg_HL(Cpuz80 *cpu);

void write_reg_SP(Cpuz80 *cpu, uint16_t value);

uint16_t read_reg_SP(Cpuz80 *cpu);

void write_reg_AF(Cpuz80 *cpu, uint16_t value);

uint16_t read_reg_AF(Cpuz80 *cpu);

#endif
