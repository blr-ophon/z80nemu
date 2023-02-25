#ifndef STACK_H
#define STACK_H
#include <stdint.h>

struct cpuz80;

void stack_push16(struct cpuz80 *cpu, uint16_t value);

uint16_t stack_pop16(struct cpuz80 *cpu);

void stack_push8(struct cpuz80 *cpu, uint8_t value);

uint8_t stack_pop8(struct cpuz80 *cpu);

#endif
