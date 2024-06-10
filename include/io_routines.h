#ifndef IO_ROUTINES_H
#define IO_ROUTINES_H

#include <stdint.h>
#include "memory.h"
#include "registerbank.h"

void io_routines_IN(Cpuz80 *cpu, uint8_t *reg_x);

void io_routines_OUT(Cpuz80 *cpu, uint8_t *reg_x);

void io_routines_IN_C(Cpuz80 *cpu, uint8_t *reg_x);

void io_routines_OUT_C(Cpuz80 *cpu, uint8_t *reg_x);

#endif
