#ifndef INSTR_IXY_H
#define INSTR_IXY_H

#include "z80.h"
#include "instructions.h"
#include "instr_ixybit.h"
#include "stack.h"

void instr_ixy(Cpuz80 *cpu, uint8_t opcode, bool iy);
void instr_ixy_decode(Cpuz80 *cpu, uint8_t opcode, bool iy);
void instr_ixy_A(Cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz, bool iy);
void instr_ixy_B(Cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz, bool iy);
void instr_ixy_C(Cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz, bool iy);

#endif
