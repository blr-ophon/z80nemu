#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

struct cpuz80;

void instruction_res_set_IXIY(struct cpuz80 *cpu, uint8_t opcode, bool bit_state, uint8_t *ixy_operand);

void instruction_bit_IXIY(struct cpuz80 *cpu, uint8_t opcode, uint8_t *ixy_operand);

void instruction_aluop_IXIY(struct cpuz80 *cpu, uint8_t opcode, bool iy_mode);

void instruction_ld_IXIY(struct cpuz80 *cpu, uint8_t opcode, bool iy_mode);

void instruction_ld(struct cpuz80 *cpu, uint8_t opcode);

void instruction_res_set(struct cpuz80 *cpu, uint8_t opcode, bool bit_state);

void instruction_bit(struct cpuz80 *cpu, uint8_t opcode);

void instruction_sla(struct cpuz80 *cpu, uint8_t *reg_x);

void instruction_sll(struct cpuz80 *cpu, uint8_t *reg_x);

void instruction_sra(struct cpuz80 *cpu, uint8_t *reg_x);

void instruction_srl(struct cpuz80 *cpu, uint8_t *reg_x);

void instruction_rl(struct cpuz80 *cpu, uint8_t *reg_x);

void instruction_rr(struct cpuz80 *cpu, uint8_t *reg_x);

void instruction_rlc(struct cpuz80 *cpu, uint8_t *reg_x);

void instruction_rrc(struct cpuz80 *cpu, uint8_t *reg_x);

void instruction_inc(struct cpuz80 *cpu, uint8_t *reg_x);

void instruction_dec(struct cpuz80 *cpu, uint8_t *reg_x);

void instruction_add(struct cpuz80 *cpu, uint8_t reg_x);

void instruction_adc(struct cpuz80 *cpu, uint8_t reg_x);

void instruction_sub(struct cpuz80 *cpu, uint8_t reg_x);

void instruction_sbc(struct cpuz80 *cpu, uint8_t reg_x);

void instruction_ana(struct cpuz80 *cpu, uint8_t reg_x);

void instruction_xra(struct cpuz80 *cpu, uint8_t reg_x);

void instruction_ora(struct cpuz80 *cpu, uint8_t reg_x);

void instruction_cmp(struct cpuz80 *cpu, uint8_t reg_x);

#endif

