#ifndef IO_ROUTINES_H
#define IO_ROUTINES_H
#include <stdint.h>

struct cpu8080;

void io_routines_IN(struct cpu8080 *cpu, uint8_t *reg_x);

void io_routines_OUT(struct cpu8080 *cpu, uint8_t *reg_x);

#endif
