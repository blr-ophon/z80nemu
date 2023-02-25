#ifndef IO_ROUTINES_H
#define IO_ROUTINES_H
#include <stdint.h>

struct cpuz80;

void io_routines_IN(struct cpuz80 *cpu, uint8_t *reg_x);

void io_routines_OUT(struct cpuz80 *cpu, uint8_t *reg_x);

#endif
