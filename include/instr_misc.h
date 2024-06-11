#ifndef INSTR_MISC_H
#define INSTR_MISC_H

#include "z80.h"
#include "io_routines.h"
#include "instructions.h"
#include "instr_misc_aux.h"

void instr_misc(Cpuz80 *cpu, uint8_t opcode);
void instr_misc_decode(Cpuz80 *cpu, uint8_t opcode);
void instr_misc_B(Cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz);

#endif
