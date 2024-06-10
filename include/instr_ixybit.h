#ifndef INSTR_IXY_BIT_H
#define INSTR_IXY_BIT_H

#include "cpu.h"
#include "conditions.h"
#include "registerbank.h"
#include "instructions.h"
#include "instr_bit.h"

void instr_ixybit(struct cpuz80 *cpu, uint8_t opcode, uint8_t Imm, bool iy);

#endif 

