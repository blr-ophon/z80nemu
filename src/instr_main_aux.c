#include "instr_main_aux.h"


//LD rp,nn
void instr_main_A_LDRPnn(struct cpuz80 *cpu, uint8_t pair_idx){
    void(*write_regPair[])(struct cpuz80 *cpu, uint16_t XImm) = {
        write_reg_BC,
        write_reg_DE,
        write_reg_HL,
        write_reg_SP,
    };
    uint16_t XImm = cpu_GetLIWord(cpu);

    write_regPair[pair_idx](cpu, XImm);
}

//ADD HL,rp
void instr_main_A_addHLRP(struct cpuz80 *cpu, uint8_t pair_idx){
    uint16_t(*read_regPair[])(struct cpuz80 *cpu) = {
        read_reg_BC,
        read_reg_DE,
        read_reg_HL,
        read_reg_SP
    };

    cpu->flags.n = 0;
    uint16_t word1 = read_reg_HL(cpu);
    uint16_t word2 = read_regPair[pair_idx](cpu);
    flags_test_C16(&cpu->flags, (uint32_t) word1 + word2);
    flags_test_H16(&cpu->flags, word1, word2, 0);
    word1 += word2;
    write_reg_HL(cpu, word1);
}

//LD (dst), A. Where dst is BC,DE or XImm
//if dst is HL index, load from HL instead of A
void instr_main_A_LDDST(struct cpuz80 *cpu, uint8_t dst_idx){
    uint16_t(*get_addr[])(struct cpuz80 *cpu) = {
        read_reg_BC,
        read_reg_DE,
        cpu_GetLIWord,
        cpu_GetLIWord
    };
    uint16_t addr = get_addr[dst_idx](cpu);

    if(dst_idx == RP1_HL_IDX){ //Exceptional behavior
        cpu->memory->memory[addr] = cpu->reg_L;
        cpu->memory->memory[addr+1] = cpu->reg_H;
    }else{
        cpu->memory->memory[addr] = cpu->reg_A;
    }
}

//LD A,(src). Where src is BC,DE or XImm
//if dst is HL index, load to HL instead of A
void instr_main_A_LDSRC(struct cpuz80 *cpu, uint8_t src_idx){
    uint16_t(*get_addr[])(struct cpuz80 *cpu) = {
        read_reg_BC,
        read_reg_DE,
        cpu_GetLIWord,
        cpu_GetLIWord
    };
    uint16_t addr = get_addr[src_idx](cpu);
    
    if(src_idx == RP1_HL_IDX){ //Exceptional behavior
        cpu->reg_L = cpu->memory->memory[addr];
        cpu->reg_H = cpu->memory->memory[addr+1];
    }else{
        cpu->reg_A = cpu->memory->memory[addr];
    }
}

/*
 * INC/DEC regPair
 * inc_dec is 0 for inc and 1 for dec
 */
void instr_main_A_INCDECRP(struct cpuz80 *cpu, uint8_t pair_idx, bool inc_dec){
    uint16_t(*read_regPair[])(struct cpuz80 *cpu) = {
        read_reg_BC,
        read_reg_DE,
        read_reg_HL,
        read_reg_SP
    };
    void(*write_regPair[])(struct cpuz80 *cpu, uint16_t XImm) = {
        write_reg_BC,
        write_reg_DE,
        write_reg_HL,
        write_reg_SP,
    };
    uint16_t increment = inc_dec? 1 : -1;

    uint16_t word = read_regPair[pair_idx](cpu); 
    write_regPair[pair_idx](cpu, word + increment);
}
