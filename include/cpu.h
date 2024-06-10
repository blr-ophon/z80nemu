#ifndef CPU_H
#define CPU_H 

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "stack.h"
#include "memory.h"
#include "flags.h"
#include "registerbank.h"
#include "instructions.h"
#include "config.h"
#include "io_routines.h"
#include "instr_main.h"

//TODO:Check that, in the program, all pins that "go out" of the cpu are
//only set/reset by the cpu emulation, while all pins that "come in" to the
//cpu are set/reset by (emulated) external devices.


void cpu_init(Cpuz80 *cpu, Memory *memory);

void cpu_cycle(Cpuz80 *cpu);

void cpu_main_instructions(Cpuz80 *cpu, uint8_t opcode);

void cpu_IXIY_instructions(Cpuz80 *cpu, uint8_t opcode, bool iy);

void cpu_misc_instructions(Cpuz80 *cpu, uint8_t opcode);

#endif
