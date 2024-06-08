#ifndef REGISTERBANK_H
#define REGISTERBANK_H
#include <stdint.h>

//TODO: switch to get/set

typedef struct cpuz80 Cpuz80;


void write_reg_BC(Cpuz80 *cpu, uint16_t value);
void write_reg_DE(Cpuz80 *cpu, uint16_t value);
void write_reg_HL(Cpuz80 *cpu, uint16_t value);
void write_reg_SP(Cpuz80 *cpu, uint16_t value);
void write_reg_AF(Cpuz80 *cpu, uint16_t value);
void write_reg_IX(Cpuz80 *cpu, uint16_t value);
void write_reg_IY(Cpuz80 *cpu, uint16_t value);

uint16_t read_reg_BC(Cpuz80 *cpu);
uint16_t read_reg_DE(Cpuz80 *cpu);
uint16_t read_reg_HL(Cpuz80 *cpu);
uint16_t read_reg_SP(Cpuz80 *cpu);
uint16_t read_reg_AF(Cpuz80 *cpu);
uint16_t read_reg_IX(Cpuz80 *cpu);
uint16_t read_reg_IY(Cpuz80 *cpu);

void write_reg_B(Cpuz80 *cpu, uint8_t value);
void write_reg_C(Cpuz80 *cpu, uint8_t value);
void write_reg_D(Cpuz80 *cpu, uint8_t value);
void write_reg_E(Cpuz80 *cpu, uint8_t value);
void write_reg_H(Cpuz80 *cpu, uint8_t value);
void write_reg_L(Cpuz80 *cpu, uint8_t value);
void write_reg_pHL(Cpuz80 *cpu, uint8_t value);
void write_reg_A(Cpuz80 *cpu, uint8_t value);

uint8_t read_reg_B(Cpuz80 *cpu);
uint8_t read_reg_C(Cpuz80 *cpu);
uint8_t read_reg_D(Cpuz80 *cpu);
uint8_t read_reg_E(Cpuz80 *cpu);
uint8_t read_reg_H(Cpuz80 *cpu);
uint8_t read_reg_L(Cpuz80 *cpu);
uint8_t read_reg_pHL(Cpuz80 *cpu);
uint8_t read_reg_A(Cpuz80 *cpu);

void write_reg_IXL(Cpuz80 *cpu, uint8_t value);
void write_reg_IXH(Cpuz80 *cpu, uint8_t value);
void write_reg_IYL(Cpuz80 *cpu, uint8_t value);
void write_reg_IYH(Cpuz80 *cpu, uint8_t value);
void write_reg_pIXd(Cpuz80 *cpu, uint8_t value);
void write_reg_pIYd(Cpuz80 *cpu, uint8_t value);

uint8_t read_reg_IXL(Cpuz80 *cpu);
uint8_t read_reg_IXH(Cpuz80 *cpu);
uint8_t read_reg_pIXd(Cpuz80 *cpu);
uint8_t read_reg_IYL(Cpuz80 *cpu);
uint8_t read_reg_IYH(Cpuz80 *cpu);
uint8_t read_reg_pIYd(Cpuz80 *cpu);
#endif
