#ifndef INSTR_MAIN_H
#define INSTR_MAIN_H

#include "instructions.h"
#include "conditions.h"
#include "instr_main_aux.h"
#include "instr_ixy.h"
#include "instr_bit.h"
#include "instr_misc.h"
#include "config.h"

void instr_main(Cpuz80 *cpu, uint8_t opcode);
void instr_main_decode(Cpuz80 *cpu, uint8_t opcode);
void instr_main_A(Cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz);
void instr_main_B(Cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz);
void instr_main_C(Cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz);
void instr_main_D(Cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz);


#endif
