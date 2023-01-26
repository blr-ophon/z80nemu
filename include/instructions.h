#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

struct cpu8080;

void instruction_inc_8(struct cpu8080 *cpu, uint8_t *reg_x, int8_t addend);

void instruction_res_set(struct cpu8080 *cpu, uint8_t opcode, bool bit_state);

void instruction_bit(struct cpu8080 *cpu, uint8_t opcode);

void instruction_sla(struct cpu8080 *cpu, uint8_t *reg_x);

void instruction_sra(struct cpu8080 *cpu, uint8_t *reg_x);

void instruction_srl(struct cpu8080 *cpu, uint8_t *reg_x);

void instruction_rl(struct cpu8080 *cpu, uint8_t *reg_x);

void instruction_rr(struct cpu8080 *cpu, uint8_t *reg_x);

void instruction_rlc(struct cpu8080 *cpu, uint8_t *reg_x);

void instruction_rrc(struct cpu8080 *cpu, uint8_t *reg_x);

void instruction_add(struct cpu8080 *cpu, uint8_t reg_x);

void instruction_adc(struct cpu8080 *cpu, uint8_t reg_x);

void instruction_sub(struct cpu8080 *cpu, uint8_t reg_x);

void instruction_sbc(struct cpu8080 *cpu, uint8_t reg_x);

void instruction_ana(struct cpu8080 *cpu, uint8_t reg_x);

void instruction_xra(struct cpu8080 *cpu, uint8_t reg_x);

void instruction_ora(struct cpu8080 *cpu, uint8_t reg_x);

void instruction_cmp(struct cpu8080 *cpu, uint8_t reg_x);

#endif

