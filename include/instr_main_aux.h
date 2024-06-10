#ifndef INSTR_MAIN_AUX_H
#define INSTR_MAIN_AUX_H

#include "registerbank.h"
#include "instructions.h"
#include "io_routines.h"
#include "stack.h"

//Group A
void instr_main_A_LDRPnn(Cpuz80 *cpu, uint8_t pair_idx);
void instr_main_A_addHLRP(Cpuz80 *cpu, uint8_t pair_idx);
void instr_main_A_LDDST(Cpuz80 *cpu, uint8_t dst_idx);
void instr_main_A_LDSRC(Cpuz80 *cpu, uint8_t src_idx);
void instr_main_A_INCDECRP(Cpuz80 *cpu, uint8_t pair_idx, bool inc_dec);
void instr_main_A_ROT(Cpuz80 *cpu, uint8_t rot_idx);
void instr_main_A_SPECIAL(Cpuz80 *cpu, uint8_t idx);

//Group D
void instr_main_D_SPECIAL(Cpuz80 *cpu, uint8_t idx);

#endif
