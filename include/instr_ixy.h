#ifndef INSTR_IXY_H
#define INSTR_IXY_H

#include "cpu.h"
#include "conditions.h"
#include "registerbank.h"
#include "instructions.h"

void instr_ixy(struct cpuz80 *cpu, uint8_t opcode, bool iy);
void instr_ixy_A(struct cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz, bool iy);
void instr_ixy_B(struct cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz, bool iy);
void instr_ixy_C(struct cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz, bool iy);

#endif
