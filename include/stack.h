#ifndef STACK_H
#define STACK_H
#include <stdint.h>

struct cpu8080;

void stack_push16(struct cpu8080 *cpu, uint16_t value);

uint16_t stack_pop16(struct cpu8080 *cpu);

void stack_push8(struct cpu8080 *cpu, uint8_t value);

uint8_t stack_pop8(struct cpu8080 *cpu);

#endif
