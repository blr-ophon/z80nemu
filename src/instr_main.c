#include "instr_main.h"

//TODO: document functions
//TODO: Use standard notation on code text, n/Imm nn/XImm
//TODO: intr_main_X: function array

/*
 * Instructions marked as Exceptional are not handled here
 */
void instr_main(Cpuz80 *cpu, uint8_t opcode){
    switch(opcode){
        case 0x00: //NOP
            break;
        case 0x08: //EX AF,AF'
            {
            uint16_t nreg_af = flags_load_byte(&cpu->flags);
            nreg_af |= ((uint16_t)cpu->reg_A) << 8;
            nreg_af = ~nreg_af;

            uint8_t reg_F = nreg_af;
            flags_sta_byte(&cpu->flags, reg_F);
            cpu->reg_A = nreg_af >> 8;
            break;
            }
        case 0xc9: //RET
            cpu->PC = stack_pop16(cpu) -1;
            break;
        case 0xcb: //PREFIX: BIT INSTRUCTIONS
            {
            uint8_t prf_opcode = memory_read8(cpu->memory, ++cpu->PC);
            instr_bit(cpu, prf_opcode);
            break;
            }
        case 0xcd: //CALL nn
            {
            uint16_t adr = z80_fetchLIWord(cpu);
            stack_push16(cpu, cpu->PC+1);
            cpu->PC = adr -1;
            break;
            }
        case 0xd9: //EXX
            write_reg_BC(cpu, ~(read_reg_BC(cpu)));
            write_reg_DE(cpu, ~(read_reg_DE(cpu)));
            write_reg_HL(cpu, ~(read_reg_HL(cpu)));
            break;
        case 0xdd: //PREFIX: IX INSTRUCTIONS
            {
            uint8_t prf_opcode = memory_read8(cpu->memory, ++cpu->PC);
            instr_ixy(cpu, prf_opcode, 0);
            break;
            }
        case 0xe9: //JP (HL)
            {
            uint16_t word = read_reg_HL(cpu);
            cpu->PC = word -1;
            break;
            }
        case 0xed: //PREFIX: MISC INTRUCTIONS
            {
            uint8_t prf_opcode = memory_read8(cpu->memory, ++cpu->PC);
            instr_misc(cpu, prf_opcode);
            break;
            }
        case 0xf9: //LD SP,HL
            {
            uint16_t word = read_reg_HL(cpu);
            cpu->SP = word;
            break;
            }
        case 0xfd: //PREFIX: IY INSTRUCTIONS
            {
            uint8_t prf_opcode = memory_read8(cpu->memory, ++cpu->PC);
            instr_ixy(cpu, prf_opcode, 1);
            break;
            }
        default:
            instr_main_decode(cpu, opcode);
            break;
    }
}


void instr_main_decode(struct cpuz80 *cpu, uint8_t opcode){
    uint8_t opcode_xx = (opcode & 0xc0) >> 6;       //1100 0000    
    uint8_t opcode_yyy = (opcode & 0x38) >> 3;      //0011 1000
    uint8_t opcode_zzz = opcode & 0x07;             //0000 0111

    void(*instr_main_groups[])(Cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz) = {
        instr_main_A,   //Group A: 0x00 to 0x3f
        instr_main_B,   //Group B: 0x40 to 0x7f
        instr_main_C,   //Group C: 0x80 to 0xbf
        instr_main_D,   //Group D: 0xc0 to 0xff
    };
    instr_main_groups[opcode_xx](cpu, opcode_yyy, opcode_zzz);
}


/*
 * zzz: type of operation
 * yyy: operation specification (condition, register etc)
 */
void instr_main_A(struct cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz){
    uint8_t *regsPtrs[] = {
        &cpu->reg_B,
        &cpu->reg_C,
        &cpu->reg_D,
        &cpu->reg_E,
        &cpu->reg_H,
        &cpu->reg_L,
        &cpu->memory->memory[read_reg_HL(cpu)],
        &cpu->reg_A
    };
    switch(opcode_zzz){
        case 0: //JR(Cond) n
                //Exceptional: NOP / EX AF,AF'
            /*
             * yyy = specify one from 6 conditions
             * 000: NOP
             * 001: EX AF,AF'
             */
            {
            bool(*condition_X[])(struct cpuz80 *cpu) = {
                NULL,   //NOP
                NULL,   //EX AF,AF'
                condition_DNZ,
                condition_TRUE,
                condition_NZ,
                condition_Z,
                condition_NC,
                condition_C,
            };
            bool condition = condition_X[opcode_yyy](cpu); 

            uint8_t Imm = memory_read8(cpu->memory, ++cpu->PC);

            if(condition){
                cpu->PC += Imm;
            }
            break;
            }
        case 1: //LD regPair, XImm      
                //ADD regPair, regPair
            /*
             * yyy = first bit (lsb) defines if LD or ADD
             * last 2 bits specify one from four pairs
             */
            {
            uint8_t op_idx = (opcode_yyy & 0x01);           //001
            uint8_t pair_idx = (opcode_yyy & 0x06) >> 1;    //110

            void(*LD_ADD_regPair[])(struct cpuz80 *cpu, uint8_t pair_idx) = {
                instr_main_A_LDRPnn,
                instr_main_A_addHLRP,
                };

            LD_ADD_regPair[op_idx](cpu, pair_idx);
            }
            break;
        case 2: //LD (dst), A               
                //LD A, (src)
            /*
             * yyy = first bit (lsd) defines LD (dst),A or LD A,(src)
             * last 2 bits specify if dst/adr is BC(0), DE(1) or XImm(2,3)
             */
            {
            uint8_t srcdst_idx = (opcode_yyy & 0x06) >> 1;    //110
            uint8_t op_idx = (opcode_yyy & 0x01);             //001
                                                              
            void(*LD_SRC_DST[])(struct cpuz80 *cpu, uint8_t srcdst_idx) = {
                instr_main_A_LDDST,
                instr_main_A_LDSRC
            };

            LD_SRC_DST[op_idx](cpu, srcdst_idx);
            break;
            }
        case 3: //INC (regPair)             
                //DEC (regPair)
            /*
             * yyy = first bit (lsd) defines INC(0) or DEC(1)
             * last 2 bits specify register pair
             */
            {
            uint8_t pair_idx = (opcode_yyy & 0x06) >> 1;      //110
            uint8_t op_idx = (opcode_yyy & 0x01);             //001
            
            instr_main_A_INCDECRP(cpu, pair_idx, op_idx);
            break;
            }
        case 4: //INC (reg)             
            /*
             * yyy = one of 8 registers
             */
            {
            instruction_inc(cpu, regsPtrs[opcode_yyy]);
            break;
            }
        case 5: //DEC (reg)             
            /*
             * yyy = one of 8 registers
             */
            {
            instruction_dec(cpu, regsPtrs[opcode_yyy]);
            break;
            }
        case 6: //LD (reg),Imm
            /*
             * yyy = one of 8 registers
             */
            {
            uint8_t val = memory_read8(cpu->memory, ++cpu->PC);
            *regsPtrs[opcode_yyy] = val;
            break;
            }
        case 7: //RLCA, RRCA, RLA, RRA
                //DAA, CPL, SCF, CCF
            /*
             * yyy: 
             * 000 - 011: Rotate
             * 100 - 111: Special 
             */
            if(opcode_yyy <= 3){ //Rotate A 
                instr_main_A_ROT(cpu, opcode_yyy);
            }else{ //SPECIAL
                instr_main_A_SPECIAL(cpu, opcode_yyy - 4);
            }
            break;
    }
}


/*
 * yyy: dst register
 * zzz: src register
 */
void instr_main_B(struct cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz){
    uint8_t *regsPtrs[] = {
        &cpu->reg_B,
        &cpu->reg_C,
        &cpu->reg_D,
        &cpu->reg_E,
        &cpu->reg_H,
        &cpu->reg_L,
        &cpu->memory->memory[read_reg_HL(cpu)],
        &cpu->reg_A
    };

    *(regsPtrs[opcode_yyy]) = *(regsPtrs[opcode_zzz]);
}


/*
 * yyy: ALU operation
 * zzz: src register
 */
void instr_main_C(struct cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz){
    uint8_t *regsPtrs[] = {
        &cpu->reg_B,
        &cpu->reg_C,
        &cpu->reg_D,
        &cpu->reg_E,
        &cpu->reg_H,
        &cpu->reg_L,
        &cpu->memory->memory[read_reg_HL(cpu)],
        &cpu->reg_A
    };

    void(*operations[])(struct cpuz80 *cpu, uint8_t reg_x) = {
        instruction_add,
        instruction_adc,
        instruction_sub,
        instruction_sbc,
        instruction_ana,
        instruction_xra,
        instruction_ora,
        instruction_cmp
    };

    operations[opcode_yyy](cpu, *regsPtrs[opcode_zzz]);
}


/*
 * zzz: Operation
 * yyy: Operation specific
 */
void instr_main_D(struct cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz){
    bool(*condition_X[])(struct cpuz80 *cpu) = {
        condition_NZ,
        condition_Z,
        condition_NC,
        condition_C,
        condition_PO,
        condition_PE,
        condition_P,
        condition_M
    };
    void(*write_regPair[])(struct cpuz80 *cpu, uint16_t XImm) = {
        write_reg_BC,
        write_reg_DE,
        write_reg_HL,
        write_reg_AF,
    };
    uint16_t(*read_regPair[])(struct cpuz80 *cpu) = {
        read_reg_BC,
        read_reg_DE,
        read_reg_HL,
        read_reg_AF
    };
    void(*operations[])(struct cpuz80 *cpu, uint8_t reg_x) = {
        instruction_add,
        instruction_adc,
        instruction_sub,
        instruction_sbc,
        instruction_ana,
        instruction_xra,
        instruction_ora,
        instruction_cmp
    };

    switch(opcode_zzz){
        case 0: //RET (cond)
            /*
             * yyy = specify one from 8 conditions
             */
            {
            if(condition_X[opcode_yyy](cpu)){
                cpu->PC = stack_pop16(cpu) -1;
            }
            break;
            }
        case 1: //POP regPair
                //Exceptional: EXX / JP (HL) / LD SP,HL
            /*
             * yyy 
             * first bit (lsb) = 0 for POP. Exceptional treated outside
             * last 2 bits specify one from four pairs
             */
            {  
            uint16_t word = stack_pop16(cpu);
            write_regPair[opcode_yyy >> 1](cpu, word);
            break;
            }
        case 2: //JP (cond)
            /*
             * yyy = specify one from 8 conditions
             */
            {
            uint16_t adr = z80_fetchLIWord(cpu);
            if(condition_X[opcode_yyy](cpu)){
                cpu->PC = adr -1;
            }
            break;
            }
        case 3: //JP nn         /   Bit
                //Out (n),a     /   in a,(in)
                //EX (SP),HL    /   EX DE,HL
                //DI            /   EI
            instr_main_D_SPECIAL(cpu, opcode_yyy);
            break;
        case 4: //CALL (cond)
            /*
             * yyy = specify one from 8 conditions
             */
            {
            uint16_t adr = z80_fetchLIWord(cpu);
            if(condition_X[opcode_yyy](cpu)){
                stack_push16(cpu, cpu->PC+1);
                cpu->PC = adr -1;
            }
            break;
            }
        case 5: //PUSH regPair
                //CALL nn
            /*
             * yyy 
             * first bit (lsb) = 0 for PUSH. Exceptional treated outside
             * last 2 bits specify one from four pairs
             */
            {
            stack_push16(cpu, read_regPair[opcode_yyy >> 1](cpu));
            break;
            }
        case 6: //ALUOP A,Imm
            /*
             * yyy = one of 8 ALU operations
             */
            {
            uint8_t Imm = memory_read8(cpu->memory, ++cpu->PC);
            operations[opcode_yyy](cpu, Imm);
            break;
            }
        case 7: //RST x
            /*
             * yyy: Vectors Addresses are multiples of 8 
             */
            stack_push16(cpu, cpu->PC+1);
            cpu->PC = (INTERRUPT_VECTOR_SIZE * opcode_yyy) - 1;
            break;
    }
}
