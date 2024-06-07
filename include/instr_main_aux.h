#ifndef INSTR_MAIN_AUX_H
#define INSTR_MAIN_AUX_H

#include "cpu.h"
#include "decoder.h"

//Group A
void instr_main_A_LDRPnn(struct cpuz80 *cpu, uint8_t pair_idx);
void instr_main_A_addHLRP(struct cpuz80 *cpu, uint8_t pair_idx);
void instr_main_A_LDDST(struct cpuz80 *cpu, uint8_t dst_idx);
void instr_main_A_LDSRC(struct cpuz80 *cpu, uint8_t src_idx);
void instr_main_A_INCDECRP(struct cpuz80 *cpu, uint8_t pair_idx, bool inc_dec);
void instr_main_A_ROT(struct cpuz80 *cpu, uint8_t rot_idx);
void instr_main_A_SPECIAL(struct cpuz80 *cpu, uint8_t idx);

#endif
