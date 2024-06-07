#ifndef INSTR_MAIN_H
#define INSTR_MAIN_H

#include "instructions.h"
#include "cpu.h"
#include "conditions.h"
#include "instr_main_aux.h"

struct cpuz80;


void instr_main(struct cpuz80 *cpu, uint8_t opcode);
void instr_main_A(struct cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz);
void instr_main_B(struct cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz);

#endif
