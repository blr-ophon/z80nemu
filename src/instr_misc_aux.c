#include "instr_misc_aux.h"

/*
 * LD (dst),regPair
 */
void instr_misc_LDDST(Cpuz80 *cpu, uint8_t rpair_idx){
    uint16_t(*read_regPair[])(Cpuz80 *cpu) = {
        read_reg_BC,
        read_reg_DE,
        read_reg_HL,
        read_reg_SP,
    };
    uint16_t XImm = z80_fetchLIWord(cpu);
    uint16_t *Ptr = (uint16_t*) &cpu->memory->memory[XImm];    
    *Ptr = read_regPair[rpair_idx](cpu);
}

/*
 * LD regPair,(src)
 */
void instr_misc_LDSRC(Cpuz80 *cpu, uint8_t rpair_idx){
    void(*write_regPair[])(Cpuz80 *cpu, uint16_t val) = {
        write_reg_BC,
        write_reg_DE,
        write_reg_HL,
        write_reg_SP,
    };
    uint16_t XImm = z80_fetchLIWord(cpu);
    uint16_t *Ptr = (uint16_t*) &cpu->memory->memory[XImm];    
    write_regPair[rpair_idx](cpu, *Ptr);
}

void instr_misc_ADC(Cpuz80 *cpu, uint16_t regPair){
    uint32_t result = read_reg_HL(cpu) + regPair + cpu->flags.cy;
    flags_test_V16(&cpu->flags, read_reg_HL(cpu), regPair + cpu->flags.cy);
    flags_test_H16(&cpu->flags, read_reg_HL(cpu), regPair, cpu->flags.cy);
    flags_test_C16(&cpu->flags, result);

    cpu->flags.z = (uint16_t)result == 0? 1 : 0;
    cpu->flags.s = result & 0x8000? 1 : 0;
    cpu->flags.n = 0;

    write_reg_HL(cpu, result);
}

void instr_misc_SBC(Cpuz80 *cpu, uint16_t regPair){
    uint8_t borrow = ~(cpu->flags.cy) & 0x01;
    uint32_t result = read_reg_HL(cpu) + ~regPair + borrow;

    cpu->flags.cy = 0;
    if((result ^ read_reg_HL(cpu) ^ ~regPair) & 0x10000){ 
        cpu->flags.cy = 1;
    }

    flags_test_V16(&cpu->flags, read_reg_HL(cpu), ~(regPair) + borrow);
    flags_test_H16(&cpu->flags, read_reg_HL(cpu), ~regPair, borrow);

    cpu->flags.z = (uint16_t)result == 0? 1 : 0;
    cpu->flags.s = result & 0x08000? 1 : 0;
    cpu->flags.n = 1;
    cpu->flags.cy = ~cpu->flags.cy & 0x01;

    write_reg_HL(cpu, result);
}
