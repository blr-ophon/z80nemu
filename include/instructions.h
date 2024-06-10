#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "registerbank.h"


void instruction_misc_adc(Cpuz80 *cpu, uint16_t reg_x);

void instruction_misc_sbc(Cpuz80 *cpu, uint16_t reg_x);

void instruction_ld(Cpuz80 *cpu, uint8_t opcode);

void instruction_res_set(Cpuz80 *cpu, uint8_t opcode, bool bit_state);

void instruction_bit(Cpuz80 *cpu, uint8_t opcode);

void instruction_sla(Cpuz80 *cpu, uint8_t *reg_x);
void instruction_sll(Cpuz80 *cpu, uint8_t *reg_x);
void instruction_sra(Cpuz80 *cpu, uint8_t *reg_x);
void instruction_srl(Cpuz80 *cpu, uint8_t *reg_x);
void instruction_rl(Cpuz80 *cpu, uint8_t *reg_x);
void instruction_rr(Cpuz80 *cpu, uint8_t *reg_x);
void instruction_rlc(Cpuz80 *cpu, uint8_t *reg_x);
void instruction_rrc(Cpuz80 *cpu, uint8_t *reg_x);

void instruction_inc(Cpuz80 *cpu, uint8_t *reg_x);
void instruction_dec(Cpuz80 *cpu, uint8_t *reg_x);

void instruction_add(Cpuz80 *cpu, uint8_t reg_x);
void instruction_adc(Cpuz80 *cpu, uint8_t reg_x);
void instruction_sub(Cpuz80 *cpu, uint8_t reg_x);
void instruction_sbc(Cpuz80 *cpu, uint8_t reg_x);
void instruction_ana(Cpuz80 *cpu, uint8_t reg_x);
void instruction_xra(Cpuz80 *cpu, uint8_t reg_x);
void instruction_ora(Cpuz80 *cpu, uint8_t reg_x);
void instruction_cmp(Cpuz80 *cpu, uint8_t reg_x);

#endif

