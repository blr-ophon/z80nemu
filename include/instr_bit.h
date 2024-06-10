#ifndef INSTR_BIT_H
#define INSTR_BIT_H

#include "instructions.h"

void instr_bit(Cpuz80 *cpu, uint8_t opcode);

void instr_bit_BIT(Cpuz80 *cpu, uint8_t n, uint8_t *reg_x);

void instr_bit_RES_SET(Cpuz80 *cpu, uint8_t n, uint8_t *reg_x, bool res_set);

void instr_bit_RLC(Cpuz80 *cpu, uint8_t *reg_x);
void instr_bit_RRC(Cpuz80 *cpu, uint8_t *reg_x);
void instr_bit_RL(Cpuz80 *cpu, uint8_t *reg_x);
void instr_bit_RR(Cpuz80 *cpu, uint8_t *reg_x);
void instr_bit_SLA(Cpuz80 *cpu, uint8_t *reg_x);
void instr_bit_SRA(Cpuz80 *cpu, uint8_t *reg_x);
void instr_bit_SLL(Cpuz80 *cpu, uint8_t *reg_x);
void instr_bit_SRL(Cpuz80 *cpu, uint8_t *reg_x);

#endif

