#ifndef STACK_H
#define STACK_H

#include "z80.h"


void stack_push16(Cpuz80 *cpu, uint16_t value);

uint16_t stack_pop16(Cpuz80 *cpu);

void stack_push8(Cpuz80 *cpu, uint8_t value);

uint8_t stack_pop8(Cpuz80 *cpu);

#endif
