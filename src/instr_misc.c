#include "instr_misc.h"

void instr_misc(Cpuz80 *cpu, uint8_t opcode){
    switch(opcode){
        case 0x44: //NEG
            {
            uint16_t result = 0 + (uint16_t)~(cpu->reg_A) + 1;
            flags_test_ZS(&cpu->flags, result);
            flags_test_H(&cpu->flags, 0, ~(cpu->reg_A), 1);
            cpu->flags.h = !cpu->flags.h;
            flags_test_V(&cpu->flags, 0, ~(cpu->reg_A), 1);
            cpu->flags.cy = 1;
            if((result ^ 0 ^ ~(cpu->reg_A)) & 0x0100){ 
                cpu->flags.cy = 0;
            }
            cpu->flags.n = 1;
            cpu->reg_A = result;
            break;
            }
        case 0x45: //RETN
                   //TODO
            break;
        case 0x46: //IM 0
            cpu->interrupt_mode = 0;
            break;
        case 0x47: //LD I,A
            cpu->reg_I = cpu->reg_A;
            break;
        case 0x4d: //RETI
                   //TODO
            break;
        case 0x4f: //LD R,A
            cpu->reg_R = cpu->reg_A;
            break;
        case 0x56: //IM 1
            cpu->interrupt_mode = 1;
            break;
        case 0x57: //LD A,I
            cpu->reg_A = cpu->reg_I;
            break;
        case 0x5e: //IM 2
            cpu->interrupt_mode = 2;
            break;
        case 0x5f: //LD A,R
            cpu->reg_A = cpu->reg_R;
            break;
        case 0x67: //RRD
            {
            uint8_t temp_reg_A = cpu->reg_A;
            cpu->reg_A = (cpu->reg_A & 0xf0) | (PTR_HL & 0x0f);
            PTR_HL >>= 4;             //logical shift right 
            PTR_HL |= (temp_reg_A) << 4;
            //flags
            cpu->flags.n = 0;
            flags_test_P(&cpu->flags, cpu->reg_A);
            cpu->flags.h = 0;
            cpu->flags.z = cpu->reg_A == 0? 1 : 0;
            cpu->flags.s = cpu->reg_A & 0x80? 1 : 0;
            break;
            }
        case 0x6f: //RLD
            {
            uint8_t temp_reg_A = cpu->reg_A;
            cpu->reg_A = (cpu->reg_A & 0xf0); 
            cpu->reg_A |= (PTR_HL & 0xf0) >> 4;
            PTR_HL = (PTR_HL << 4) | (temp_reg_A & 0x0f);
            //flags
            cpu->flags.n = 0;
            flags_test_P(&cpu->flags, cpu->reg_A);
            cpu->flags.h = 0;
            cpu->flags.z = cpu->reg_A == 0? 1 : 0;
            cpu->flags.s = cpu->reg_A & 0x80? 1 : 0;
            break;
            }
        case 0xa0: //LDI
            PTR_DE = PTR_HL;
            write_reg_DE(cpu, read_reg_DE(cpu) + 1);
            write_reg_HL(cpu, read_reg_HL(cpu) + 1);
            write_reg_BC(cpu, read_reg_BC(cpu) - 1);
            cpu->flags.h = 0;
            cpu->flags.p = read_reg_BC(cpu) != 0? 1 : 0;
            cpu->flags.n = 0;
            break;
        case 0xa1: //CPI
            {
            const uint8_t subtrahend = PTR_HL;
            const uint8_t result = cpu->reg_A + ~subtrahend + 1;
            flags_test_ZS(&cpu->flags, result);
            flags_test_H(&cpu->flags, cpu->reg_A, ~subtrahend, 1);
            cpu->flags.h = !cpu->flags.h;
            write_reg_HL(cpu, read_reg_HL(cpu) + 1);
            write_reg_BC(cpu, read_reg_BC(cpu) - 1);
            cpu->flags.p = read_reg_BC(cpu) != 0? 1 : 0;
            cpu->flags.n = 1;
            break;
            }
        case 0xa2: //INI
                   //TODO
            break;
        case 0xa3: //OUTI
                   //TODO
            break;
        case 0xa8: //LDD
            PTR_DE = PTR_HL;
            write_reg_DE(cpu, read_reg_DE(cpu) - 1);
            write_reg_HL(cpu, read_reg_HL(cpu) - 1);
            write_reg_BC(cpu, read_reg_BC(cpu) - 1);
            cpu->flags.h = 0;
            cpu->flags.p = read_reg_BC(cpu) != 0? 1 : 0;
            cpu->flags.n = 0;
            break;
        case 0xa9: //CPD
            {
            const uint8_t subtrahend = PTR_HL;
            const uint8_t result = cpu->reg_A + ~subtrahend + 1;
            flags_test_ZS(&cpu->flags, result);
            flags_test_H(&cpu->flags, cpu->reg_A, ~subtrahend, 1);
            cpu->flags.h = !cpu->flags.h;
            write_reg_HL(cpu, read_reg_HL(cpu) - 1);
            write_reg_BC(cpu, read_reg_BC(cpu) - 1);
            cpu->flags.p = read_reg_BC(cpu) != 0? 1 : 0;
            cpu->flags.n = 1;
            break;
            }
        case 0xaa: //IND
                   //TODO
            break;
        case 0xab: //OUTD
                   //TODO
            break;
        case 0xb0: //LDIR
            PTR_DE = PTR_HL;
            write_reg_DE(cpu, read_reg_DE(cpu) + 1);
            write_reg_HL(cpu, read_reg_HL(cpu) + 1);
            write_reg_BC(cpu, read_reg_BC(cpu) - 1);
            if(read_reg_BC(cpu) != 0){
                cpu->PC -= 2;
                //Instruction repeats and interrupts are recognized
            }
            cpu->flags.h = 0;
            cpu->flags.p = 0;
            cpu->flags.n = 0;
            break;
        case 0xb1: //CPIR
            {
            uint8_t subtrahend = PTR_HL;
            uint8_t result = cpu->reg_A - subtrahend;
            flags_test_ZS(&cpu->flags, result);
            flags_test_H(&cpu->flags, cpu->reg_A, ~subtrahend, 1);
            cpu->flags.h = !cpu->flags.h;
            cpu->flags.n = 1;
            write_reg_HL(cpu, read_reg_HL(cpu) + 1);
            write_reg_BC(cpu, read_reg_BC(cpu) - 1);
            cpu->flags.p = read_reg_BC(cpu) != 0? 1 : 0;
            if(cpu->flags.p && !cpu->flags.z){
                cpu->PC -= 2;
            }
            break;
            }
        case 0xb2: //INIR
                   //TODO
            break;
        case 0xb3: //OTIR 
                   //TODO
            break;
        case 0xb8: //LDDR
            PTR_DE = PTR_HL;
            write_reg_DE(cpu, read_reg_DE(cpu) - 1);
            write_reg_HL(cpu, read_reg_HL(cpu) - 1);
            write_reg_BC(cpu, read_reg_BC(cpu) - 1);
            if(read_reg_BC(cpu) != 0){
                cpu->PC -= 2;
                //Instruction repeats and interrupts are recognized
            }
            cpu->flags.h = 0;
            cpu->flags.p = 0;
            cpu->flags.n = 0;
            break;
        case 0xb9: //CPDR
            {
            uint8_t subtrahend = PTR_HL;
            uint8_t result = cpu->reg_A - subtrahend;
            flags_test_ZS(&cpu->flags, result);
            flags_test_H(&cpu->flags, cpu->reg_A, ~subtrahend, 1);
            cpu->flags.h = !cpu->flags.h;
            cpu->flags.n = 1;
            write_reg_HL(cpu, read_reg_HL(cpu) - 1);
            write_reg_BC(cpu, read_reg_BC(cpu) - 1);
            cpu->flags.p = read_reg_BC(cpu) != 0? 1 : 0;
            if(cpu->flags.p && !cpu->flags.z){
                cpu->PC -= 2;
            }
            break;
            }
        case 0xba: //INDR
                   //TODO
            break;
        case 0xbb: //OTDR
                   //TODO
            break;
        default:
            instr_misc_decode(cpu, opcode);
    }
}


void instr_misc_decode(Cpuz80 *cpu, uint8_t opcode){
    uint8_t opcode_xx = (opcode & 0xc0) >> 6;       //1100 0000    
    uint8_t opcode_yyy = (opcode & 0x38) >> 3;      //0011 1000
    uint8_t opcode_zzz = opcode & 0x07;             //0000 0111
    switch(opcode_xx){
        case 0: //Group A
            break;
        case 1: //Group B
            instr_misc_B(cpu, opcode_yyy, opcode_zzz);
            break;
        case 2: //Group C
            //instr_ixy_C(cpu, opcode_yyy, opcode_zzz, iy);
            break;
        case 3: //Group D. Fully Exceptional.
            break;
    }
}

void instr_misc_B(Cpuz80 *cpu, uint8_t opcode_yyy, uint8_t opcode_zzz){
    uint8_t dummyReg = 0;
    uint8_t *regsPtrs[] = {
        &cpu->reg_B,
        &cpu->reg_C,
        &cpu->reg_D,
        &cpu->reg_E,
        &cpu->reg_H,
        &cpu->reg_L,
        &dummyReg,      //Receives value on IN. Sends 0 on OUT
        &cpu->reg_A
    };
    uint16_t(*read_regPair[])(struct cpuz80 *cpu) = {
        read_reg_BC,
        read_reg_DE,
        read_reg_HL,
        read_reg_SP
    };

    switch(opcode_zzz){
        case 0: //IN
            io_routines_IN_C(cpu, regsPtrs[opcode_yyy]);
            break;
        case 1: //OUT
            io_routines_OUT_C(cpu, regsPtrs[opcode_yyy]);
            break;
        case 2: //SBC regPair,regPair
                //ADC regPair,regPair
            /*
             * yyy = first bit (lsb) defines SBC(0) or ADC(1)
             * last 2 bits specify regPair used
             */
            {
            uint8_t pair_idx = (opcode_yyy & 0x06) >> 1;      //110
            uint8_t op_idx = (opcode_yyy & 0x01);             //001
                                                              
            void(*SBC_ADC[])(struct cpuz80 *cpu, uint16_t regPair) = {
                instr_misc_SBC,
                instr_misc_ADC
            };
            SBC_ADC[op_idx](cpu, read_regPair[pair_idx](cpu));

            break;
            }
        case 3: //LD (dst), RP
                //LD RP, (src)
            /*
             * yyy = first bit (lsd) defines LD (dst),regPair or LD regPair,(src)
             * last 2 bits specify regPair used
             */
            {
            uint8_t srcdst_idx = (opcode_yyy & 0x06) >> 1;    //110
            uint8_t op_idx = (opcode_yyy & 0x01);             //001
                                                              
            void(*LD_SRC_DST[])(struct cpuz80 *cpu, uint8_t srcdst_idx) = {
                instr_misc_LDDST,
                instr_misc_LDSRC
            };

            LD_SRC_DST[op_idx](cpu, srcdst_idx);
            break;
            }
        case 4: //Exceptional
            break;
        case 5: //Exceptional
            break;
        case 6: //Exceptional
            break;
        case 7: //Exceptional
            break;
    }
}
