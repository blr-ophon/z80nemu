#include "instr_main_aux.h"


//LD rp,nn
void instr_main_A_LDRPnn(Cpuz80 *cpu, uint8_t pair_idx){
    void(*write_regPair[])(Cpuz80 *cpu, uint16_t XImm) = {
        write_reg_BC,
        write_reg_DE,
        write_reg_HL,
        write_reg_SP,
    };
    uint16_t XImm = z80_fetchLIWord(cpu);

    write_regPair[pair_idx](cpu, XImm);
}

//ADD HL,rp
void instr_main_A_addHLRP(Cpuz80 *cpu, uint8_t pair_idx){
    uint16_t(*read_regPair[])(Cpuz80 *cpu) = {
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
void instr_main_A_LDDST(Cpuz80 *cpu, uint8_t dst_idx){
    uint16_t(*get_addr[])(Cpuz80 *cpu) = {
        read_reg_BC,
        read_reg_DE,
        z80_fetchLIWord,
        z80_fetchLIWord
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
void instr_main_A_LDSRC(Cpuz80 *cpu, uint8_t src_idx){
    uint16_t(*get_addr[])(Cpuz80 *cpu) = {
        read_reg_BC,
        read_reg_DE,
        z80_fetchLIWord,
        z80_fetchLIWord
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
void instr_main_A_INCDECRP(Cpuz80 *cpu, uint8_t pair_idx, bool inc_dec){
    uint16_t(*read_regPair[])(Cpuz80 *cpu) = {
        read_reg_BC,
        read_reg_DE,
        read_reg_HL,
        read_reg_SP
    };
    void(*write_regPair[])(Cpuz80 *cpu, uint16_t XImm) = {
        write_reg_BC,
        write_reg_DE,
        write_reg_HL,
        write_reg_SP,
    };
    uint16_t increment = inc_dec? -1 : 1;

    uint16_t word = read_regPair[pair_idx](cpu); 
    write_regPair[pair_idx](cpu, word + increment);
}

//Rotate instructions
void instr_main_A_ROT(Cpuz80 *cpu, uint8_t rot_idx){
    void(*ROT_A[])(Cpuz80 *cpu, uint8_t *reg) = {
        instruction_rlc,
        instruction_rrc,
        instruction_rl,
        instruction_rr
    };
    ROT_A[rot_idx](cpu, &cpu->reg_A);
}
static inline void instr_main_A_DAA(Cpuz80 *cpu){
    uint8_t correction = 0;

    uint8_t ls_nibble = cpu->reg_A & 0x0f;
    if((ls_nibble > 0x09) || cpu->flags.h){
        correction += 0x06;
    }

    uint8_t ms_nibble = cpu->reg_A & 0xf0;
    if((ms_nibble > 0x90) || cpu->flags.cy || (ms_nibble >= 0x90 && ls_nibble > 9)){
        correction += 0x60;
        cpu->flags.cy = 1; //if no overflow, remais unchanged, not reset
    }

    if(cpu->flags.n){
        cpu->flags.h = cpu->flags.h && (cpu->reg_A & 0x0F) < 0x06;
        cpu->reg_A  -= correction;
    }else{
        cpu->flags.h = (cpu->reg_A & 0x0F) > 0x09;
        cpu->reg_A += correction;
      }
    flags_test_ZS(&cpu->flags, cpu->reg_A);
    flags_test_P(&cpu->flags, cpu->reg_A);
}

void instr_main_A_SPECIAL(Cpuz80 *cpu, uint8_t idx){
    //DAA, CPL, SCF, CCF
    switch(idx){
        case 0: //DAA
            instr_main_A_DAA(cpu);
            break;
        case 1: //CPL
            cpu->flags.n = 1;
            cpu->flags.h = 1;
            cpu->reg_A = ~cpu->reg_A;
            break;
        case 2: //SCF
            cpu->flags.h = 0;
            cpu->flags.n = 0;
            cpu->flags.cy = 1;
            break;
        case 3: //CCF
            cpu->flags.h = cpu->flags.cy;
            cpu->flags.n = 0;
            cpu->flags.cy = !cpu->flags.cy;
            break;
    }
}

void instr_main_D_SPECIAL(Cpuz80 *cpu, uint8_t idx){
    //DAA, CPL, SCF, CCF
    switch(idx){
        case 0: //JP nn
            {
            uint16_t adr = z80_fetchLIWord(cpu);
            cpu->PC = adr -1;
            break;
            }
        case 1: //Exceptional: Bit Instructions
            break;
        case 2: //OUT (n),A
            io_routines_OUT(cpu, &cpu->reg_A);
            break;
        case 3: //IN A,(n)
            io_routines_IN(cpu, &cpu->reg_A);
            break;
        case 4: //EX (SP),HL
            {
            uint16_t word = stack_pop16(cpu);
            stack_push16(cpu, read_reg_HL(cpu));
            write_reg_HL(cpu, word);
            break;
            }
        case 5: //EX DE,HL
            {
            uint16_t temp = read_reg_HL(cpu);
            write_reg_HL(cpu, read_reg_DE(cpu));
            write_reg_DE(cpu, temp);
            break;
            }
        case 6: //DI
            cpu->interrupt_enable = 0;
            break;
        case 7: //EI
            cpu->interrupt_enable = 1;
            break;
    }
}

