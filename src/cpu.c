#include "cpu.h"
#include "instr_main.h"
#include "instr_ixy.h"
#include "instr_bit.h"
#include "instr_ixybit.h"
//TODO: create a common header for cpu.h, flags.h, instructions.h and io_routines.h
//to avoid redefinition of structs errors
//TODO: CPU speed
//TODO modularize this code into cpuIXIY cpubit and such
//to make debug easier
//TODO: proper PC increment: save a copy of (pc), increment pc
//and execute the copy. this simulates the pc being saved in instruction
//register before being incremented. This should be done last, as it breaks
//byte fetchs, pushs, jumps, calls, in, out etc

static void unimplemented_opcode(Cpuz80 *cpu, uint8_t *opcode){
    fprintf(stderr, "Unimplemented Opcode: %02X\n", *opcode);
    fprintf(stderr, "Step: %d\n", cpu->step_count);
    exit(1);
}

uint16_t cpu_GetLIWord(Cpuz80 *cpu){
    uint16_t word = cpu->memory->memory[++cpu->PC];
    word ^= (uint16_t) cpu->memory->memory[++cpu->PC] << 8;
    return word;
}


void cpu_init(Cpuz80 *cpu, Memory *memory){
    //Clear cpu registers
    memset(cpu, 0, sizeof(*cpu));
    //memset(used_opcodes, 0xff, sizeof(used_opcodes)); //USED in DEBUG log
    cpu->step_count = 0; //should be cycle count
    //INTERNAL REGISTERS
    cpu->interrupt_enable = 1;
    cpu->interrupt_mode = 0;
    //PINS
    cpu->M_pin = 1;
    cpu->RD_pin = 1;
    cpu->WR_pin = 1;
    cpu->IORQ_pin = 1;
    cpu->MREQ_pin = 1;
    cpu->INT_pin = 1;
    cpu->NMI_pin = 1;
    cpu->memory = memory;
    //TODO: custom PC initialization
    cpu->PC = 0x0100;
}

void cpu_int_handler(Cpuz80 *cpu){
    switch(cpu->interrupt_mode){
        case 0: //IM 0
            break;
        case 1: //IM 0
            break;
        case 2: //IM 0
            break;
    }

}

void cpu_cycle(Cpuz80 *cpu){
    cpu->M_pin = 0;     //this is set by io instructions to avoid interrupts
                        //and DMAs during execution in 2 parts
    //fetch
    uint8_t opcode = memory_read8(cpu->memory, cpu->PC);
    cpu_exec_instruction(cpu, &opcode);
    cpu->PC ++;
    cpu->step_count++;

    //Check for interrupts
    if(!cpu->INT_pin && cpu->interrupt_enable && !cpu->M_pin){ 
        cpu_int_handler(cpu);
    }
    //Check for bus requests
}

void cpu_exec_instruction(Cpuz80 *cpu, uint8_t *opcode){
    switch(*opcode){
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
        case 0xcd: //CALL nn
            {
            uint16_t adr = cpu_GetLIWord(cpu);
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
            cpu_IXIY_instructions(cpu, &prf_opcode, 0);
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
            cpu_misc_instructions(cpu, prf_opcode);
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
            cpu_IXIY_instructions(cpu, &prf_opcode, 1);
            break;
            }
        default:
            instr_main(cpu, *opcode);
            break;
    }
}

void cpu_bit_instructions(Cpuz80 *cpu, uint8_t *opcode){
    instr_bit(cpu, *opcode);
}

void cpu_IXIY_instructions(Cpuz80 *cpu, uint8_t *opcode, bool iy){
    uint16_t *pIXY = iy? &cpu->reg_IY : &cpu->reg_IX;
    switch(*opcode){
        case 0x21: //LD IX/Y,nn
            *pIXY = cpu_GetLIWord(cpu);
            break;
        case 0x22: //LD (nn),IX/Y
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            cpu->memory->memory[adr] = *pIXY; 
            cpu->memory->memory[adr+1] = (*pIXY & 0xff00) >> 8;
            break;
            }
        case 0x23: //INC IX/Y
            (*pIXY) ++;
            break;
        case 0x2a: //LD IX/Y,(nn)
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            *pIXY = 0;
            *pIXY |= cpu->memory->memory[adr];
            *pIXY |= (uint16_t)(cpu->memory->memory[adr+1]) << 8;
            break;
            }
        case 0x2b: //DEC IX/Y
            (*pIXY) --;
            break;
        case 0xcb: //PREFIX: IX/Y BIT INSTRUCTIONS
            {
            uint8_t Imm = memory_read8(cpu->memory, ++cpu->PC);
            uint8_t prf_opcode = memory_read8(cpu->memory, ++cpu->PC);
            instr_ixybit(cpu, prf_opcode, Imm, iy);
            break;
            }
        case 0xe1: //POP IX/Y
            (*pIXY) = stack_pop16(cpu);
            break;
        case 0xe3: //EX (SP),IX/Y
            {
            uint16_t temp = cpu->SP;
            cpu->SP = (*pIXY);
            (*pIXY) = temp;
            break;
            }
        case 0xe5: //PUSH IX/Y
            stack_push16(cpu, (*pIXY));
            break;
        case 0xe9: //JP (IX/Y)
            cpu->PC = (*pIXY) - 1;
            break;
        case 0xf9: //LD SP,IX/Y
            cpu->SP = (*pIXY);
            break;
        default:
            instr_ixy(cpu, *opcode, iy);
            break;
    }
}

void cpu_misc_instructions(Cpuz80 *cpu, uint8_t opcode){
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
            uint16_t adr = cpu_GetLIWord(cpu);
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
            uint16_t adr = cpu_GetLIWord(cpu);
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
            uint16_t adr = cpu_GetLIWord(cpu);
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
            uint16_t adr = cpu_GetLIWord(cpu);
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
            uint16_t adr = cpu_GetLIWord(cpu);
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
            uint16_t adr = cpu_GetLIWord(cpu);
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
