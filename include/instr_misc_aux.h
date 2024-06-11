#ifndef INSTR_MISC_AUX_H
#define INSTR_MISC_AUX_H

#include "registerbank.h"
#include "flags.h"

void instr_misc_ADC(Cpuz80 *cpu, uint16_t regPair);
void instr_misc_SBC(Cpuz80 *cpu, uint16_t regPair);
void instr_misc_LDDST(Cpuz80 *cpu, uint8_t rpair_idx);
void instr_misc_LDSRC(Cpuz80 *cpu, uint8_t rpair_idx);

#endif
