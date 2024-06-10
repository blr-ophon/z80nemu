#include "instr_misc.h"

void instr_misc(Cpuz80 *cpu, uint8_t opcode){
    switch(opcode){
        case 0x40: //IN B,(c)
            io_routines_IN_C(cpu, &cpu->reg_B);
            break;
        case 0x41: //OUT (c),B
            io_routines_OUT_C(cpu, &cpu->reg_B);
            break;
        case 0x42: //SBC HL,BC
            instruction_misc_sbc(cpu, read_reg_BC(cpu));
            break;
        case 0x43: //LD (nn),BC
            {
            uint16_t adr = z80_fetchLIWord(cpu);
            cpu->memory->memory[adr] = cpu->reg_C;
            cpu->memory->memory[adr+1] = cpu->reg_B;
            break;
            }
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
        case 0x48: //IN C,(C)
            io_routines_IN(cpu, &cpu->reg_C);
            break;
        case 0x49: //OUT (C),C
            io_routines_OUT(cpu, &cpu->reg_C);
            break;
        case 0x4a: //ADC HL,BC
            instruction_misc_adc(cpu, read_reg_BC(cpu));
            break;
        case 0x4b: //LD BC,(nn)
            {
            uint16_t adr = z80_fetchLIWord(cpu);
            cpu->reg_C = cpu->memory->memory[adr];
            cpu->reg_B = cpu->memory->memory[adr+1];
            break;
            }
        case 0x4d: //RETI
                   //TODO
            break;
        case 0x4f: //LD R,A
            cpu->reg_R = cpu->reg_A;
            break;
        case 0x50: //IN D,(c)
            io_routines_IN(cpu, &cpu->reg_D);
            break;
        case 0x51: //OUT (c),D
            io_routines_OUT(cpu, &cpu->reg_D);
            break;
        case 0x52: //SBC HL,DE
            instruction_misc_sbc(cpu, read_reg_DE(cpu));
            break;
        case 0x53: //LD (nn),DE
            {
            uint16_t adr = z80_fetchLIWord(cpu);
            cpu->memory->memory[adr] = cpu->reg_E;
            cpu->memory->memory[adr+1] = cpu->reg_D;
            break;
            }
        case 0x56: //IM 1
            cpu->interrupt_mode = 1;
            break;
        case 0x57: //LD A,I
            cpu->reg_A = cpu->reg_I;
            break;
        case 0x58: //IN E,(C)
            io_routines_IN(cpu, &cpu->reg_E);
            break;
        case 0x59: //OUT (C),E
            io_routines_OUT(cpu, &cpu->reg_E);
            break;
        case 0x5a: //ADC HL,DE
            instruction_misc_adc(cpu, read_reg_DE(cpu));
            break;
        case 0x5b: //LD DE,(nn)
            {
            uint16_t adr = z80_fetchLIWord(cpu);
            cpu->reg_E = cpu->memory->memory[adr];
            cpu->reg_D = cpu->memory->memory[adr+1];
            break;
            }
            break;
        case 0x5e: //IM 2
            cpu->interrupt_mode = 2;
            break;
        case 0x5f: //LD A,R
            cpu->reg_A = cpu->reg_R;
            break;
        case 0x60: //IN H,(c)
            io_routines_IN(cpu, &cpu->reg_H);
            break;
        case 0x61: //OUT (c),H
            io_routines_OUT(cpu, &cpu->reg_H);
            break;
        case 0x62: //SBC HL,HL
            instruction_misc_sbc(cpu, read_reg_HL(cpu));
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
        case 0x68: //IN L,(c)
            io_routines_IN(cpu, &cpu->reg_L);
            break;
        case 0x69: //OUT (c),L
            io_routines_OUT(cpu, &cpu->reg_L);
            break;
        case 0x6a: //ADC HL,HL
            instruction_misc_adc(cpu, read_reg_HL(cpu));
            break;
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
        case 0x72: //SBC HL,SP
            instruction_misc_sbc(cpu, cpu->SP);
            break;
        case 0x73: //LD (nn),SP
            {
            uint16_t adr = z80_fetchLIWord(cpu);
            cpu->memory->memory[adr] = cpu->SP & 0x00ff;
            cpu->memory->memory[adr+1] = (cpu->SP & 0xff00) >> 8;
            break;
            }
        case 0x78: //IN A,(c)
            io_routines_IN(cpu, &cpu->reg_A);
            break;
        case 0x79: //OUT (c),A
            io_routines_OUT(cpu, &cpu->reg_A);
            break;
        case 0x7a: //ADC HL,SP
            instruction_misc_adc(cpu, cpu->SP);
            break;
        case 0x7b: //LD SP,(nn)
            {
            uint16_t new_SP = 0;
            uint16_t adr = z80_fetchLIWord(cpu);
            new_SP |= cpu->memory->memory[adr];
            new_SP |= (cpu->memory->memory[adr+1]) << 8;
            cpu->SP = new_SP;
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
    }
}
