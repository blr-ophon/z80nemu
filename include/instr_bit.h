#ifndef INSTR_BIT_H
#define INSTR_BIT_H

#include "instructions.h"
#include "decoder.h"

void instr_bit(struct cpuz80 *cpu, uint8_t opcode);

void instr_bit_BIT(struct cpuz80 *cpu, uint8_t n, uint8_t *reg_x);

void instr_bit_RES_SET(struct cpuz80 *cpu, uint8_t n, uint8_t *reg_x, bool res_set);

void instr_bit_RLC(struct cpuz80 *cpu, uint8_t *reg_x);
void instr_bit_RRC(struct cpuz80 *cpu, uint8_t *reg_x);
void instr_bit_RL(struct cpuz80 *cpu, uint8_t *reg_x);
void instr_bit_RR(struct cpuz80 *cpu, uint8_t *reg_x);
void instr_bit_SLA(struct cpuz80 *cpu, uint8_t *reg_x);
void instr_bit_SRA(struct cpuz80 *cpu, uint8_t *reg_x);
void instr_bit_SLL(struct cpuz80 *cpu, uint8_t *reg_x);
void instr_bit_SRL(struct cpuz80 *cpu, uint8_t *reg_x);

#endif

