#include "instr_ixy.h"

void instr_ixy(struct cpuz80 *cpu, uint8_t opcode, bool iy){
    uint8_t opcode_xx = (opcode & 0xc0) >> 6;       //1100 0000    
    uint8_t opcode_yyy = (opcode & 0x38) >> 3;      //0011 1000
    uint8_t opcode_zzz = opcode & 0x07;             //0000 0111
    switch(opcode_xx){
        case 0: //Group A
            instr_ixy_A(cpu, opcode_yyy, opcode_zzz, iy);
            break;
        case 1: //Group B
            instr_ixy_B(cpu, opcode_yyy, opcode_zzz, iy);
            break;
        case 2: //Group C
            instr_ixy_C(cpu, opcode_yyy, opcode_zzz, iy);
            break;
        case 3: //Group D. Fully Exceptional.
            break;
    }
}

static inline void instr_ixy_add(struct cpuz80 *cpu, uint16_t *pIXY, uint16_t reg_pair){
    //TODO: Check PC increment in all prefix instructions
    //TODO: Fix this instruction and send it to instructions.c
    uint32_t result = *pIXY + reg_pair;
    cpu->flags.cy = result & 0x10000? 1: 0;
    cpu->flags.n = 0;
    flags_test_H16(&cpu->flags, *pIXY, reg_pair, 0);
    *pIXY = result; 
}

/*
 * zzz: Operation
 * yyy: Operation Specific
 */
void instr_ixy_A(struct cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz, bool iy){
    uint16_t *pIXY = iy? &cpu->reg_IY : &cpu->reg_IX;

    uint16_t(*read_regPair[])(struct cpuz80 *cpu) = {
        read_reg_BC,
        read_reg_DE,
        NULL,
        read_reg_SP
    };
    read_regPair[2] = iy? read_reg_IY : read_reg_IX;

    uint8_t *regsPtrs[] = {
        &cpu->reg_B,
        &cpu->reg_C,
        &cpu->reg_D,
        &cpu->reg_E,
        (uint8_t*)pIXY+1,       //IXH
        (uint8_t*)pIXY,         //IXL
        NULL,
        &cpu->reg_A
    };
    if(opcode_yyy == 6){
        //Fetch next byte only if ix/y+d is an operand
        uint16_t adr = (*pIXY) + (int16_t) memory_read8(cpu->memory, ++cpu->PC);
        regsPtrs[6] = &cpu->memory->memory[adr];
    }


    switch(opcode_zzz){
        case 0: //Unused
            break;
        case 1: //Exceptional: LD IX,nn         yyy = 100
                //ADD IXY,regPair               yyy = xx1
            instr_ixy_add(cpu, pIXY, read_regPair[opcode_yyy >> 1](cpu));
            break;
        case 2: //Exceptional. Mostly Unused
            break;
        case 3: //Exceptional. Mostly Unused
            break;
        case 4: //INC reg
            instruction_inc(cpu, regsPtrs[opcode_yyy]);
            break;
        case 5: //DEC reg
            instruction_dec(cpu, regsPtrs[opcode_yyy]);
            break;
        case 6: //LD reg,n
            {
            uint16_t Imm = memory_read8(cpu->memory, ++cpu->PC);
            *regsPtrs[opcode_yyy] = Imm;
            break;
            }
        case 7: //Unused
            break;
    }
}

/*
 * LD reg,reg 
 * yyy: Dst register
 * zzz: Src register
 */
void instr_ixy_B(struct cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz, bool iy){
    uint16_t *pIXY = iy? &cpu->reg_IY : &cpu->reg_IX;

    uint8_t *regsPtrs[] = {
        &cpu->reg_B,
        &cpu->reg_C,
        &cpu->reg_D,
        &cpu->reg_E,
        (uint8_t*)pIXY+1,       //IXH
        (uint8_t*)pIXY,         //IXL
        NULL,
        &cpu->reg_A
    };

    if(opcode_yyy == 0x6 || opcode_zzz == 0x6){ 
        //Fetch next byte only if ix/y+d is an operand
        uint16_t adr = (*pIXY) + (int16_t) memory_read8(cpu->memory, ++cpu->PC);
        regsPtrs[6] = &cpu->memory->memory[adr];
        //(ix/y + d) is an operand. ixyh and ixyl become H and L
        regsPtrs[5] = &cpu->reg_L;
        regsPtrs[4] = &cpu->reg_H;
    }

    //LD (*targetPtr),(*sourcePtr) 
    *regsPtrs[opcode_yyy] = *regsPtrs[opcode_zzz];
}


/*
 * ALU operations
 * yyy: Operation
 * zzz: Src Register
 */
void instr_ixy_C(struct cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz, bool iy){
    uint16_t *pIXY = iy? &cpu->reg_IY : &cpu->reg_IX;

    uint8_t *regsPtrs[] = {
        &cpu->reg_B,
        &cpu->reg_C,
        &cpu->reg_D,
        &cpu->reg_E,
        (uint8_t*)pIXY+1,       //IXH
        (uint8_t*)pIXY,         //IXL
        NULL,
        &cpu->reg_A
    };
    if(opcode_zzz == 0x6){ 
        //Fetch next byte only if ix/y+d is an operand
        uint16_t adr = (*pIXY) + (int16_t) memory_read8(cpu->memory, ++cpu->PC);
        regsPtrs[6] = &cpu->memory->memory[adr];
    }

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
