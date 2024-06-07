#include "instr_main.h"
#include "registerbank.h"

//TODO: document functions

void instr_main(struct cpuz80 *cpu, uint8_t opcode){
    uint8_t opcode_xx = (opcode & 0xc0) >> 6;       //1100 0000    
    uint8_t opcode_yyy = (opcode & 0x38) >> 3;      //0011 1000
    uint8_t opcode_zzz = opcode & 0x07;             //0000 0111

    switch(opcode_xx){
        case 0: //Group A
            instr_main_A(cpu, opcode_yyy, opcode_zzz);
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
    }
}


/*
 * zzz: type of operation
 * yyy: operation specification (condition, register etc)
 */
void instr_main_A(struct cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz){
    switch(opcode_zzz){
        case 0: //NOP, EX AF,AF', JR(Cond) n
            /*
             * yyy = specify one from 6 conditions
             * 000: NOP
             * 001: EX AF,AF'
             */
            {
            if(opcode_yyy == 0) //NOP
                break;
            if(opcode_yyy == 1){ //EX AF,AF'
                uint16_t nreg_af = flags_load_byte(&cpu->flags);
                nreg_af |= ((uint16_t)cpu->reg_A) << 8;
                nreg_af = ~nreg_af;

                uint8_t reg_F = nreg_af;
                flags_sta_byte(&cpu->flags, reg_F);
                cpu->reg_A = nreg_af >> 8;
                break;
            }

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
            }
            break;
        case 1: //LD regPair, XImm      
                //ADD regPair, regPair
            /*
             * yyy = first bit defines if LD or ADD
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
             * yyy = first bit defines LD (dst),A or LD A,(src)
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
             * yyy = first bit defines INC(0) or DEC(1)
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
            instruction_inc(cpu, regsPtrs[opcode_yyy]);
            break;
            }
        case 5: //DEC (reg)             
            /*
             * yyy = one of 8 registers
             */
            {
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
            instruction_dec(cpu, regsPtrs[opcode_yyy]);
            break;
            }
        case 6: //LD (reg),Imm
            /*
             * yyy = one of 8 registers
             */
            {
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


