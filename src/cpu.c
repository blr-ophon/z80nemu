#include "cpu.h"
//TODO: create a common header for cpu.h, flags.h, instructions.h and io_routines.h
//to avoid redefinition of structs errors
//TODO: CPU speed
//TODO modularize this code into cpuIXIY cpubit and such
//to make debug easier
//TODO: proper PC increment: save a copy of (pc), increment pc
//and execute the copy. this simulates the pc being saved in instruction
//register before being incremented. This should be done last, as it breaks
//byte fetchs, pushs, jumps, calls, in, out etc

static void unimplemented_opcode(Cpu8080 *cpu, uint8_t *opcode){
    fprintf(stderr, "Unimplemented Opcode: %02X\n", *opcode);
    fprintf(stderr, "Step: %d\n", cpu->step_count);
    exit(1);
}

uint16_t cpu_GetLIWord(Cpu8080 *cpu){
    uint16_t word = cpu->memory->memory[++cpu->PC];
    word ^= (uint16_t) cpu->memory->memory[++cpu->PC] << 8;
    return word;
}


void cpu_init(Cpu8080 *cpu, Memory *memory){
    //Clear cpu registers
    memset(cpu, 0, sizeof(*cpu));
    //memset(used_opcodes, 0xff, sizeof(used_opcodes)); //USED in DEBUG log
    cpu->step_count = 0; //should be cycle count
    cpu->memory = memory;
}

void cpu_cycle(Cpu8080 *cpu){
    //Check for interrupts and bus requests
    if(cpu->INT_pin && cpu->interrupt_enable){ //RST, byte
    }

    //fetch
    uint8_t opcode = memory_read8(cpu->memory, cpu->PC);
    cpu_exec_instruction(cpu, &opcode);
    cpu->PC ++;
    cpu->step_count++;
}

void cpu_exec_instruction(Cpu8080 *cpu, uint8_t *opcode){
    switch(*opcode){
        case 0x00: //NOP
            break;
        case 0x01: //LD BC,nn
            {
            uint16_t word = cpu_GetLIWord(cpu);
            write_reg_BC(cpu,word);
            break;
            }
        case 0x02: //LD (BC),A 
            {
            uint16_t adr = read_reg_BC(cpu);
            cpu->memory->memory[adr] = cpu->reg_A;
            break;
            }
        case 0x03: //INC BC 
            {
            uint16_t word = read_reg_BC(cpu);
            word ++;
            write_reg_BC(cpu, word);
            break;
            }
        case 0x04: //INC B 
            instruction_inc_8(cpu, &cpu->reg_B, 1);
            break;
        case 0x05: //DEC B 
            instruction_inc_8(cpu, &cpu->reg_B, -1);
            break;
        case 0x06: //MVI B,n 
            cpu->reg_B = memory_read8(cpu->memory, ++cpu->PC);
            break;
        case 0x07: //RLCA
            instruction_rlc(cpu, &cpu->reg_A);
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
        case 0x09: //ADD HL, BC
            {
            cpu->flags.n = 0;
            uint16_t word1 = read_reg_HL(cpu);
            uint16_t word2 = read_reg_BC(cpu);
            flags_test_C16(&cpu->flags, (uint32_t) word1 + word2);
            flags_test_H16(&cpu->flags, word1, word2, 0);
            word1 += word2;
            write_reg_HL(cpu, word1);
            break;
            }
        case 0x0a: //LD A,(BC)
            {
            uint16_t adr = read_reg_BC(cpu);
            cpu->reg_A = cpu->memory->memory[adr];
            break;
            }
        case 0x0b: //DEC BC
            {
            uint16_t word = read_reg_BC(cpu);
            write_reg_BC(cpu, word - 1);
            break;
            }
        case 0x0c: //INC C 
            instruction_inc_8(cpu, &cpu->reg_C, 1);
            break;
        case 0x0d: //DEC C 
            instruction_inc_8(cpu, &cpu->reg_C, -1);
            break;
        case 0x0e: //LD C,n 
            cpu->reg_C = memory_read8(cpu->memory, ++cpu->PC);
            break;
        case 0x0f: //RRCA
            instruction_rrc(cpu, &cpu->reg_A);
            break;
        case 0x10: //DJNZ d
            //Does a relative jump
            cpu->reg_B --;
            if(cpu->reg_B != 0){
                cpu->PC += memory_read8(cpu->memory, ++cpu->PC);
            }
            break;
        case 0x11: //LD DE,nn 
            {
            uint16_t word = cpu_GetLIWord(cpu);
            write_reg_DE(cpu, word);
            break;
            }
        case 0x12: //LD (DE),A 
            {
            uint16_t adr = read_reg_DE(cpu);
            cpu->memory->memory[adr] = cpu->reg_A;
            break;
            }
        case 0x13: //INC DE
            {
            uint16_t word = read_reg_DE(cpu) + 1;
            write_reg_DE(cpu, word);
            break;
            }
        case 0x14: //INC D 
            instruction_inc_8(cpu, &cpu->reg_D, 1);
            break;
        case 0x15: //DEC D 
            instruction_inc_8(cpu, &cpu->reg_D, -1);
            break;
        case 0x16: //LD D,n
            cpu->reg_D = memory_read8(cpu->memory, ++cpu->PC);
            break;
        case 0x17: //RLA
            instruction_rl(cpu, &cpu->reg_A);
            break;
        case 0x18: //JR d
            //Does a relative jump
            cpu->PC += memory_read8(cpu->memory, ++cpu->PC);
            break;
        case 0x19: //ADD HL, DE
            {
            cpu->flags.n = 0;
            uint16_t word1 = read_reg_HL(cpu);
            uint16_t word2 = read_reg_DE(cpu);
            flags_test_C16(&cpu->flags, (uint32_t) word1 + word2);
            flags_test_H16(&cpu->flags, word1, word2, 0);
            word1 += word2;
            write_reg_HL(cpu, word1);
            break;
            }
        case 0x1a: //LD A,(DE)
            {
            uint16_t adr = read_reg_DE(cpu);
            cpu->reg_A = cpu->memory->memory[adr];
            break;
            }
        case 0x1b: //DEC DE
            {
            uint16_t word = read_reg_DE(cpu);
            write_reg_DE(cpu, word - 1);
            break;
            }
        case 0x1c: //INC E 
            instruction_inc_8(cpu, &cpu->reg_E, 1);
            break;
        case 0x1d: //DEC E 
            instruction_inc_8(cpu, &cpu->reg_E, -1);
            break;
        case 0x1e: //LD E,n
            cpu->reg_E = memory_read8(cpu->memory, ++cpu->PC);
            break;
        case 0x1f: //RRA
            instruction_rr(cpu, &cpu->reg_A);
            break;
        case 0x20: //JR NZ,d
            //Does a relative jump
            if(!cpu->flags.z){
                cpu->PC += memory_read8(cpu->memory, ++cpu->PC);
            }
            break;
        case 0x21: //LD HL,nn 
            {
            uint16_t word = cpu_GetLIWord(cpu);
            write_reg_HL(cpu, word);
            break;
            }
        case 0x22: //LD (nn),HL
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            cpu->memory->memory[adr] = cpu->reg_L;
            cpu->memory->memory[adr+1] = cpu->reg_H;
            break;
            }
        case 0x23: //INC HL
            {
            uint16_t word = read_reg_HL(cpu) + 1;
            write_reg_HL(cpu, word);
            break;
            }
        case 0x24: //INC H 
            instruction_inc_8(cpu, &cpu->reg_H, 1);
            break;
        case 0x25: //DEC H 
            instruction_inc_8(cpu, &cpu->reg_H, -1);
            break;
        case 0x26: //MVI H,n 
            cpu->reg_H = memory_read8(cpu->memory, ++cpu->PC);
            break;
        case 0x27: //DAA 
            {
            bool cy = cpu->flags.cy;
            uint8_t correction = 0;

            uint8_t ls_nibble = cpu->reg_A & 0x0f;
            if((ls_nibble > 0x09) || cpu->flags.h){
                correction += 0x06;
            }

            uint8_t ms_nibble = cpu->reg_A & 0xf0;
            if((ms_nibble > 0x90) || cpu->flags.cy || (ms_nibble >= 0x90 && ls_nibble > 9)){
                correction += 0x60;
                cy = 1; //if no overflow, remais unchanged, not reset
            }
            
            instruction_add(cpu, correction);
            cpu->flags.cy = cy;
            break;
            }
        case 0x28: //JR Z,d
            //Does a relative jump
            if(cpu->flags.z){
                cpu->PC += memory_read8(cpu->memory, ++cpu->PC);
            }
            break;
        case 0x29: //ADD HL, HL
            {
            cpu->flags.n = 0;
            uint16_t word = read_reg_HL(cpu);
            flags_test_C16(&cpu->flags, (uint32_t) word + word);
            flags_test_H16(&cpu->flags, word, word, 0);
            word += word;
            write_reg_HL(cpu, word);
            break;
            }
        case 0x2a: //LD HL,(nn)
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            cpu->reg_L = cpu->memory->memory[adr];
            cpu->reg_H = cpu->memory->memory[adr+1];
            break;
            }
        case 0x2b: //DEC HL
            {
            uint16_t word = read_reg_HL(cpu);
            write_reg_HL(cpu, word - 1);
            break;
            }
        case 0x2c: //INC L
            instruction_inc_8(cpu, &cpu->reg_L, 1);
            break;
        case 0x2d: //DEC L
            instruction_inc_8(cpu, &cpu->reg_L, -1);
            break;
        case 0x2e: //MVI L,n 
            cpu->reg_L = memory_read8(cpu->memory, ++cpu->PC);
            break;
        case 0x2f: //CPL
            cpu->flags.n = 1;
            cpu->flags.h = 1;
            cpu->reg_A = ~cpu->reg_A;
            break;
        case 0x30: //JR NC,d
            //Does a relative jump
            if(!cpu->flags.cy){
                cpu->PC += memory_read8(cpu->memory, ++cpu->PC);
            }
            break;
        case 0x31: //LD SP,nn 
            {
            uint16_t word = cpu_GetLIWord(cpu);
            cpu->SP = word;
            break;
            }
        case 0x32: //LD (nn),A
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            cpu->memory->memory[adr] = cpu->reg_A;
            break;
            }
        case 0x33: //INC SP
            cpu->SP ++;
            break;
        case 0x34: //INC (HL)
            instruction_inc_8(cpu, &PTR(HL), 1);
            break;
        case 0x35: //DEC (HL)
            instruction_inc_8(cpu, &PTR(HL), -1);
            break;
        case 0x36: //LD (HL),n
            cpu->memory->memory[read_reg_HL(cpu)] = memory_read8(cpu->memory, ++cpu->PC);
            break;
        case 0x37: //SCF
            cpu->flags.n = 0;
            cpu->flags.cy = 1;
            break;
        case 0x38: //JR C,d
            //Does a relative jump
            if(cpu->flags.cy){
                cpu->PC += memory_read8(cpu->memory, ++cpu->PC);
            }
            break;
        case 0x39: //ADD HL, SP
            {
            cpu->flags.n = 0;
            uint16_t word = read_reg_HL(cpu);
            flags_test_C16(&cpu->flags, (uint32_t) word + cpu->SP);
            flags_test_H16(&cpu->flags, word, cpu->SP, 0);
            word += cpu->SP;
            write_reg_HL(cpu, word);
            break;
            }
        case 0x3a: //LD A,(nn)
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            cpu->reg_A = cpu->memory->memory[adr];
            break;
            }
        case 0x3b: //DEC SP
            cpu->SP --;
            break;
        case 0x3c: //INC A
            instruction_inc_8(cpu, &cpu->reg_A, 1);
            break;
        case 0x3d: //DEC A
            instruction_inc_8(cpu, &cpu->reg_A, -1);
            break;
        case 0x3e: //LD A,n
            cpu->reg_A = memory_read8(cpu->memory, ++cpu->PC);
            break;
        case 0x3f: //CCF
            cpu->flags.n = 0;
            cpu->flags.h = cpu->flags.cy;
            cpu->flags.cy = ~cpu->flags.cy;
            break;
        case 0x40: //LD B,B 
            //TODO: use instruction_ld from here onwards
            cpu->reg_B = cpu->reg_B;
            break;
        case 0x41: //LD B,C
            cpu->reg_B = cpu->reg_C;
            break;
        case 0x42: //LD B,D 
            cpu->reg_B = cpu->reg_D;
            break;
        case 0x43: //LD B,E 
            cpu->reg_B = cpu->reg_E;
            break;
        case 0x44: //LD B,H 
            cpu->reg_B = cpu->reg_H;
            break;
        case 0x45: //LD B,L 
            cpu->reg_B = cpu->reg_L;
            break;
        case 0x46: //LD B,(HL)
            cpu->reg_B = PTR(HL);
            break;
        case 0x47: //LD B,A 
            cpu->reg_B = cpu->reg_A;
            break;
        case 0x48: //LD C,B 
            cpu->reg_C = cpu->reg_B;
            break;
        case 0x49: //LD C,C 
            cpu->reg_C = cpu->reg_C;
            break;
        case 0x4a: //LD C,D 
            cpu->reg_C = cpu->reg_D;
            break;
        case 0x4b: //LD C,E 
            cpu->reg_C = cpu->reg_E;
            break;
        case 0x4c: //LD C,H 
            cpu->reg_C = cpu->reg_H;
            break;
        case 0x4d: //LD C,L
            cpu->reg_C = cpu->reg_L;
            break;
        case 0x4e: //LD C,(HL)
            cpu->reg_C = PTR(HL);
            break;
        case 0x4f: //LD C,A 
            cpu->reg_C = cpu->reg_A;
            break;
        case 0x50: //LD D,B 
            cpu->reg_D = cpu->reg_B;
            break;
        case 0x51: //LD D,C 
            cpu->reg_D = cpu->reg_C;
            break;
        case 0x52: //LD D,D 
            cpu->reg_D = cpu->reg_D;
            break;
        case 0x53: //LD D,E 
            cpu->reg_D = cpu->reg_E;
            break;
        case 0x54: //LD D,H 
            cpu->reg_D = cpu->reg_H;
            break;
        case 0x55: //LD D,L 
            cpu->reg_D = cpu->reg_L;
            break;
        case 0x56: //LD D,(HL)
            cpu->reg_D = PTR(HL);
            break;
        case 0x57: //LD D,A 
            cpu->reg_D = cpu->reg_A;
            break;
        case 0x58: //LD E,B 
            cpu->reg_E = cpu->reg_B;
            break;
        case 0x59: //LD E,C 
            cpu->reg_E = cpu->reg_C;
            break;
        case 0x5a: //LD E,D 
            cpu->reg_E = cpu->reg_D;
            break;
        case 0x5b: //LD E,E 
            cpu->reg_E = cpu->reg_E;
            break;
        case 0x5c: //LD E,H 
            cpu->reg_E = cpu->reg_H;
            break;
        case 0x5d: //LD E,L 
            cpu->reg_E = cpu->reg_L;
            break;
        case 0x5e: //LD E,(HL)
            cpu->reg_E = PTR(HL);
            break;
        case 0x5f: //LD E,A 
            cpu->reg_E = cpu->reg_A;
            break;
        case 0x60: //LD H,B 
            cpu->reg_H = cpu->reg_B;
            break;
        case 0x61: //LD H,C
            cpu->reg_H = cpu->reg_C;
            break;
        case 0x62: //LD H,D
            cpu->reg_H = cpu->reg_D;
            break;
        case 0x63: //LD H,E
            cpu->reg_H = cpu->reg_E;
            break;
        case 0x64: //LD H,H
            cpu->reg_H = cpu->reg_H;
            break;
        case 0x65: //LD H,L
            cpu->reg_H = cpu->reg_L;
            break;
        case 0x66: //LD H,(HL)
            cpu->reg_H = PTR(HL);
            break;
        case 0x67: //LD H,A
            cpu->reg_H = cpu->reg_A;
            break;
        case 0x68: //LD L,B
            cpu->reg_L = cpu->reg_B;
            break;
        case 0x69: //LD L,C 
            cpu->reg_L = cpu->reg_C;
            break;
        case 0x6a: //LD L,D 
            cpu->reg_L = cpu->reg_D;
            break;
        case 0x6b: //LD L,E 
            cpu->reg_L = cpu->reg_E;
            break;
        case 0x6c: //LD L,H
            cpu->reg_L = cpu->reg_H;
            break;
        case 0x6d: //LD L,L
            cpu->reg_L = cpu->reg_L;
            break;
        case 0x6e: //LD L,(HL)
            cpu->reg_L = PTR(HL);
            break;
        case 0x6f: //LD L,A
            cpu->reg_L = cpu->reg_A;
            break;
        case 0x70: //LD (HL),B
            cpu->memory->memory[read_reg_HL(cpu)] = cpu->reg_B;
            break;
        case 0x71: //LD (HL),C 
            cpu->memory->memory[read_reg_HL(cpu)] = cpu->reg_C;
            break;
        case 0x72: //LD (HL),D 
            cpu->memory->memory[read_reg_HL(cpu)] = cpu->reg_D;
            break;
        case 0x73: //LD (HL),E
            cpu->memory->memory[read_reg_HL(cpu)] = cpu->reg_E;
            break;
        case 0x74: //LD (HL),H
            cpu->memory->memory[read_reg_HL(cpu)] = cpu->reg_H;
            break;
        case 0x75: //LD (HL),L 
            cpu->memory->memory[read_reg_HL(cpu)] = cpu->reg_L;
            break;
        case 0x76: //HALT
            //TODO
            break;
        case 0x77: //LD (HL),A 
            cpu->memory->memory[read_reg_HL(cpu)] = cpu->reg_A;
            break;
        case 0x78: //LD A,B 
            cpu->reg_A = cpu->reg_B;
            break;
        case 0x79: //LD A,C 
            cpu->reg_A = cpu->reg_C;
            break;
        case 0x7a: //LD A,D 
            cpu->reg_A = cpu->reg_D;
            break;
        case 0x7b: //LD A,E 
            cpu->reg_A = cpu->reg_E;
            break;
        case 0x7c: //LD A,H
            cpu->reg_A = cpu->reg_H;
            break;
        case 0x7d: //LD A,L
            cpu->reg_A = cpu->reg_L;
            break;
        case 0x7e: //LD A,(HL)
            cpu->reg_A = PTR(HL);
            break;
        case 0x7f: //LD A,A
            cpu->reg_A = cpu->reg_A;
            break;
        case 0x80: //ADD A,B
            instruction_add(cpu, cpu->reg_B);
            break;
        case 0x81: //ADD A,C
            instruction_add(cpu, cpu->reg_C);
            break;
        case 0x82: //ADD A,D
            instruction_add(cpu, cpu->reg_D);
            break;
        case 0x83: //ADD A,E
            instruction_add(cpu, cpu->reg_E);
            break;
        case 0x84: //ADD A,H
            instruction_add(cpu, cpu->reg_H);
            break;
        case 0x85: //ADD A,L
            instruction_add(cpu, cpu->reg_L);
            break;
        case 0x86: //ADD A,(HL)
            {
            instruction_add(cpu, PTR(HL));
            break;
            }
        case 0x87: //ADD A,A
            instruction_add(cpu, cpu->reg_A);
            break;
        case 0x88: //ADC A,B
            instruction_adc(cpu, cpu->reg_B);
            break;
        case 0x89: //ADC A,C
            instruction_adc(cpu, cpu->reg_C);
            break;
        case 0x8a: //ADC A,D
            instruction_adc(cpu, cpu->reg_D);
            break;
        case 0x8b: //ADC A,E
            instruction_adc(cpu, cpu->reg_E);
            break;
        case 0x8c: //ADC A,H
            instruction_adc(cpu, cpu->reg_H);
            break;
        case 0x8d: //ADC A,L
            instruction_adc(cpu, cpu->reg_L);
            break;
        case 0x8e: //ADC A,(HL)
            instruction_adc(cpu, PTR(HL));
            break;
        case 0x8f: //ADC A,A
            instruction_adc(cpu, cpu->reg_A);
            break;
        case 0x90: //SUB A,B
            instruction_sub(cpu, cpu->reg_B);
            break;
        case 0x91: //SUB A,C
            instruction_sub(cpu, cpu->reg_C);
            break;
        case 0x92: //SUB A,D
            instruction_sub(cpu, cpu->reg_D);
            break;
        case 0x93: //SUB A,E
            instruction_sub(cpu, cpu->reg_E);
            break;
        case 0x94: //SUB A,H
            instruction_sub(cpu, cpu->reg_H);
            break;
        case 0x95: //SUB A,L
            instruction_sub(cpu, cpu->reg_L);
            break;
        case 0x96: //SUB A,(HL)
            instruction_sub(cpu, PTR(HL));
            break;
        case 0x97: //SUB A,A
            instruction_sub(cpu, cpu->reg_A);
            break;
        case 0x98: //SBC A,B
            instruction_sbc(cpu, cpu->reg_B);
            break;
        case 0x99: //SBC A,C
            instruction_sbc(cpu, cpu->reg_C);
            break;
        case 0x9a: //SBC A,D
            instruction_sbc(cpu, cpu->reg_D);
            break;
        case 0x9b: //SBC A,E
            instruction_sbc(cpu, cpu->reg_E);
            break;
        case 0x9c: //SBC A,H
            instruction_sbc(cpu, cpu->reg_H);
            break;
        case 0x9d: //SBC A,L
            instruction_sbc(cpu, cpu->reg_L);
            break;
        case 0x9e: //SBC A,(HL)
            instruction_sbc(cpu, PTR(HL));
            break;
        case 0x9f: //SBC A,A
            instruction_sbc(cpu, cpu->reg_A);
            break;
        case 0xa0: //AND B
            instruction_ana(cpu, cpu->reg_B);
            break;
        case 0xa1: //AND C
            instruction_ana(cpu, cpu->reg_C);
            break;
        case 0xa2: //AND D
            instruction_ana(cpu, cpu->reg_D);
            break;
        case 0xa3: //AND E
            instruction_ana(cpu, cpu->reg_E);
            break;
        case 0xa4: //AND H
            instruction_ana(cpu, cpu->reg_H);
            break;
        case 0xa5: //AND L
            instruction_ana(cpu, cpu->reg_L);
            break;
        case 0xa6: //AND (HL)
            instruction_ana(cpu, PTR(HL));
            break;
        case 0xa7: //AND A
            instruction_ana(cpu, cpu->reg_A);
            break;
        case 0xa8: //XOR B
            instruction_xra(cpu, cpu->reg_B);
            break;
        case 0xa9: //XOR C
            instruction_xra(cpu, cpu->reg_C);
            break;
        case 0xaa: //XOR D
            instruction_xra(cpu, cpu->reg_D);
            break;
        case 0xab: //XOR E
            instruction_xra(cpu, cpu->reg_E);
            break;
        case 0xac: //XOR H
            instruction_xra(cpu, cpu->reg_H);
            break;
        case 0xad: //XOR L
            instruction_xra(cpu, cpu->reg_L);
            break;
        case 0xae: //XOR (HL)
            instruction_xra(cpu, PTR(HL));
            break;
        case 0xaf: //XOR A
            instruction_xra(cpu, cpu->reg_A);
            break;
        case 0xb0: //ORA B
            instruction_ora(cpu, cpu->reg_B);
            break;
        case 0xb1: //ORA C
            instruction_ora(cpu, cpu->reg_C);
            break;
        case 0xb2: //ORA D
            instruction_ora(cpu, cpu->reg_D);
            break;
        case 0xb3: //ORA E
            instruction_ora(cpu, cpu->reg_E);
            break;
        case 0xb4: //ORA H
            instruction_ora(cpu, cpu->reg_H);
            break;
        case 0xb5: //ORA L
            instruction_ora(cpu, cpu->reg_L);
            break;
        case 0xb6: //ORA (HL)
            instruction_ora(cpu, PTR(HL));
            break;
        case 0xb7: //ORA A
            instruction_ora(cpu, cpu->reg_A);
            break;
        case 0xb8: //CMP B
            instruction_cmp(cpu, cpu->reg_B);
            break;
        case 0xb9: //CMP C
            instruction_cmp(cpu, cpu->reg_C);
            break;
        case 0xba: //CMP D
            instruction_cmp(cpu, cpu->reg_D);
            break;
        case 0xbb: //CMP E
            instruction_cmp(cpu, cpu->reg_E);
            break;
        case 0xbc: //CMP H
            instruction_cmp(cpu, cpu->reg_H);
            break;
        case 0xbd: //CMP L
            instruction_cmp(cpu, cpu->reg_L);
            break;
        case 0xbe: //CMP (HL)
            instruction_cmp(cpu, PTR(HL));
            break;
        case 0xbf: //CMP A
            instruction_cmp(cpu, cpu->reg_A);
            break;
        case 0xc0: //RET NZ
            if(!cpu->flags.z){
                cpu->PC = stack_pop16(cpu) -1;
            }
            break;
        case 0xc1: //POP BC
            {
            uint16_t word = stack_pop16(cpu);
            write_reg_BC(cpu, word);
            break;
            }
        case 0xc2: //JP NZ,nn
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(!cpu->flags.z){
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xc3: //JP nn
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            cpu->PC = adr -1;
            break;
            }
        case 0xc4: //CALL NZ,nn
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(!cpu->flags.z){
                stack_push16(cpu, cpu->PC+1);
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xc5: //PUSH BC
            {
            uint16_t word = read_reg_BC(cpu);
            stack_push16(cpu, word);
            break;
            }
        case 0xc6: //ADD A,n
            {
            uint8_t operand = memory_read8(cpu->memory, ++cpu->PC);
            instruction_add(cpu, operand);
            break;
            }
        case 0xc7: //RST 00H
            stack_push16(cpu, cpu->PC+1);
            cpu->PC = (INTERRUPT_VECTOR_SIZE * 0) - 1;
            break;
        case 0xc8: //RET Z
            if(cpu->flags.z){
                cpu->PC = stack_pop16(cpu) -1;
            }
            break;
        case 0xc9: //RET
            cpu->PC = stack_pop16(cpu) -1;
            break;
        case 0xca: //JP Z,nn
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(cpu->flags.z){
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xcb: //PREFIX: BIT INSTRUCTIONS
            {
            uint8_t opcode = memory_read8(cpu->memory, ++cpu->PC);
            cpu_bit_instructions(cpu, &opcode);
            break;
            }
        case 0xcc: //CALL Z,nn
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(cpu->flags.z){
                stack_push16(cpu, cpu->PC+1);
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xcd: //CALL nn
            {
            //TODO: Fix
            uint16_t adr = cpu_GetLIWord(cpu);
            if(adr == 0x0000){
                printf("end of test\n");
                exit(1);
            }else if(adr == 0x0005){
                if(cpu->reg_C == 0x02){
                    printf("%c", cpu->reg_E);
                }
                if(cpu->reg_C == 0x09){
                    uint16_t adr = read_reg_DE(cpu);
                    char letter = cpu->memory->memory[adr];
                    while(letter != '$'){
                        letter = cpu->memory->memory[adr];
                        printf("%c", letter);
                        adr++;
                    }
                    //printf("\n");
                    /*
                    if(cpu->step_count > 10){
                        printf("STEP: %d", cpu->step_count - 6);
                        exit(1);
                    }
                    */
                }
            }else{
            stack_push16(cpu, cpu->PC+1);
            cpu->PC = adr -1;
            }
            break;
            }
        case 0xce: //ADC A,n
            {
            uint8_t operand = memory_read8(cpu->memory, ++cpu->PC); 
            instruction_adc(cpu, operand);
            break;
            }
        case 0xcf: //RST 08H
            stack_push16(cpu, cpu->PC+1);
            cpu->PC = (INTERRUPT_VECTOR_SIZE * 1) - 1;
            break;
        case 0xd0: //RET NC
            if(!cpu->flags.cy){
                cpu->PC = stack_pop16(cpu) -1;
            }
            break;
        case 0xd1: //POP DE
            {
            uint16_t word = stack_pop16(cpu);
            write_reg_DE(cpu, word);
            break;
            }
        case 0xd2: //JPC NC,nn
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(!cpu->flags.cy){
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xd3: //OUT (n),A
            {
            //duplicated byte on address bus
            uint16_t adr = (uint16_t) memory_read8(cpu->memory, ++cpu->PC);
            adr |= adr << 8;
            *(cpu->address_bus) = adr; 
            //reg_A on data_bus
            *(cpu->data_bus) = cpu->reg_A;
            cpu->IORQ_pin = 0;  //active low 
            cpu->WR_pin = 0;    //active low 
            cpu->RD_pin = 1;    //active low
            break;
            }
        case 0xd4: //CALL NC,nn
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(!cpu->flags.cy){
                stack_push16(cpu, cpu->PC+1);
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xd5: //PUSH DE
            stack_push16(cpu, read_reg_DE(cpu));
            break;
        case 0xd6: //SUB n
            {
            uint8_t operand = memory_read8(cpu->memory, ++cpu->PC); 
            instruction_sub(cpu, operand);
            break;
            }
        case 0xd7: //RST 10H;
            stack_push16(cpu, cpu->PC+1);
            cpu->PC = (INTERRUPT_VECTOR_SIZE * 2) - 1;
            break;
        case 0xd8: //RET C
            if(cpu->flags.cy){
                cpu->PC = stack_pop16(cpu) -1;
            }
            break;
        case 0xd9: //EXX
            write_reg_BC(cpu, ~(read_reg_BC(cpu)));
            write_reg_DE(cpu, ~(read_reg_DE(cpu)));
            write_reg_HL(cpu, ~(read_reg_HL(cpu)));
            break;
        case 0xda: //JP C,nn
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(cpu->flags.cy){
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xdb: //IN A,(n)
            {
            uint16_t adr = (uint16_t) memory_read8(cpu->memory, ++cpu->PC);
            adr |= adr << 8;
            *(cpu->address_bus) = adr; 
            cpu->IORQ_pin = 0;  //active low 
            cpu->WR_pin = 1;    //active low 
            cpu->RD_pin = 0;    //active low
            break;
            }
        case 0xdc: //CALL C,nn
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(cpu->flags.cy){
                stack_push16(cpu, cpu->PC+1);
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xdd: //PREFIX: IX INSTRUCTIONS
            {
            uint8_t opcode = memory_read8(cpu->memory, ++cpu->PC);
            cpu_IXIY_instructions(cpu, &opcode, 0);
            break;
            }
        case 0xde: //SBC A,n
            {
            uint8_t operand = memory_read8(cpu->memory, ++cpu->PC); 
            instruction_sbc(cpu, operand);
            break;
            }
        case 0xdf: //RST 18H;
            stack_push16(cpu, cpu->PC+1);
            cpu->PC = (INTERRUPT_VECTOR_SIZE * 3) - 1;
            break;
        case 0xe0: //RET PO
            if(!cpu->flags.p){
                cpu->PC = stack_pop16(cpu) -1;
            }
            break;
        case 0xe1: //POP HL
            {
            uint16_t word = stack_pop16(cpu);
            write_reg_HL(cpu, word);
            break;
            }
        case 0xe2: //JP PO nn
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(!cpu->flags.p){
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xe3: //EX (SP),HL
            {
            uint16_t word = stack_pop16(cpu);
            stack_push16(cpu, read_reg_HL(cpu));
            write_reg_HL(cpu, word);
            break;
            }
        case 0xe4: //CALL PO,nn
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(!cpu->flags.p){
                stack_push16(cpu, cpu->PC+1);
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xe5: //PUSH HL
            stack_push16(cpu, read_reg_HL(cpu));
            break;
        case 0xe6: //AND n
            {
            uint8_t operand = memory_read8(cpu->memory, ++cpu->PC); 
            instruction_ana(cpu, operand);
            break;
            }
        case 0xe7: //RST 20H;
            stack_push16(cpu, cpu->PC+1);
            cpu->PC = (INTERRUPT_VECTOR_SIZE * 4) - 1;
            break;
        case 0xe8: //RET PE
            if(cpu->flags.p){
                cpu->PC = stack_pop16(cpu) -1;
            }
            break;
        case 0xe9: //JP (HL)
            {
            uint16_t word = read_reg_HL(cpu);
            cpu->PC = word -1;
            break;
            }
        case 0xea: //JP PE,nn
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(cpu->flags.p){
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xeb: //EX DE,HL
            {
            uint16_t temp = read_reg_HL(cpu);
            write_reg_HL(cpu, read_reg_DE(cpu));
            write_reg_DE(cpu, temp);
            break;
            }
        case 0xec: //CALL PE,nn
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(cpu->flags.p){
                stack_push16(cpu, cpu->PC+1);
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xee: //XOR n
            {
            uint8_t operand = memory_read8(cpu->memory, ++cpu->PC); 
            instruction_xra(cpu, operand);
            break;
            }
        case 0xef: //RST 28H
            stack_push16(cpu, cpu->PC+1);
            cpu->PC = (INTERRUPT_VECTOR_SIZE * 5) - 1;
            break;
        case 0xf0: //RET P
            if(!cpu->flags.s){
                cpu->PC = stack_pop16(cpu) -1;
            }
            break;
        case 0xf1: //POP AF
            {
            uint8_t flags_reg = stack_pop8(cpu);
            flags_sta_byte(&cpu->flags, flags_reg);
            cpu->reg_A = stack_pop8(cpu);
            break;
            }
        case 0xf2: //JP P,nn
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(!cpu->flags.s){
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xf3: //DI
            cpu->interrupt_enable = 0;
            break;
        case 0xf4: //CALL P,nn
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(!cpu->flags.s){
                stack_push16(cpu, cpu->PC+1);
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xf5: //PUSH AF
            {
            uint8_t flags_reg = flags_load_byte(&cpu->flags);
            stack_push8(cpu, cpu->reg_A);
            stack_push8(cpu, flags_reg);
            break;
            }
        case 0xf6: //OR n
            {
            uint8_t operand = memory_read8(cpu->memory, ++cpu->PC); 
            instruction_ora(cpu, operand);
            break;
            }
        case 0xf7: //RST 30H
            stack_push16(cpu, cpu->PC+1);
            cpu->PC = (INTERRUPT_VECTOR_SIZE * 6) - 1;
            break;
        case 0xf8: //RET M
            if(cpu->flags.s){
                cpu->PC = stack_pop16(cpu) -1;
            }
            break;
        case 0xf9: //LD SP,HL
            {
            uint16_t word = read_reg_HL(cpu);
            cpu->SP = word;
            break;
            }
        case 0xfa: //JP M,nn
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(cpu->flags.s){
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xfb: //EI
            cpu->interrupt_enable = 1;
            break;
        case 0xfc: //CALL M,nn
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(cpu->flags.s){
                stack_push16(cpu, cpu->PC+1);
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xfe: //CP n
            {
            uint8_t operand = memory_read8(cpu->memory, ++cpu->PC);
            instruction_cmp(cpu, operand);
            break;
            }
        case 0xff: //RST 38H
            stack_push16(cpu, cpu->PC + 1);
            cpu->PC = (INTERRUPT_VECTOR_SIZE * 7) - 1;
            break;
        default:
            unimplemented_opcode(cpu,opcode);
            break;
    }
}

void cpu_bit_instructions(Cpu8080 *cpu, uint8_t *opcode){
    //TODO: use pointer to functions to reduce lines of code here
    switch(*opcode){
        //TODO: Check proper CPU Flags handling
        case 0x00: //RLC B
            instruction_rlc(cpu, &cpu->reg_B);
            flags_test_ZS(&cpu->flags, cpu->reg_B);
            flags_test_P(&cpu->flags, cpu->reg_B);
            break;
        case 0x01: //RLC C
            instruction_rlc(cpu, &cpu->reg_C);
            flags_test_ZS(&cpu->flags, cpu->reg_C);
            flags_test_P(&cpu->flags, cpu->reg_C);
            break;
        case 0x02: //RLC D
            instruction_rlc(cpu, &cpu->reg_D);
            flags_test_ZS(&cpu->flags, cpu->reg_D);
            flags_test_P(&cpu->flags, cpu->reg_D);
            break;
        case 0x03: //RLC E
            instruction_rlc(cpu, &cpu->reg_E);
            flags_test_ZS(&cpu->flags, cpu->reg_E);
            flags_test_P(&cpu->flags, cpu->reg_E);
            break;
        case 0x04: //RLC H
            instruction_rlc(cpu, &cpu->reg_H);
            flags_test_ZS(&cpu->flags, cpu->reg_H);
            flags_test_P(&cpu->flags, cpu->reg_H);
            break;
        case 0x05: //RLC L
            instruction_rlc(cpu, &cpu->reg_L);
            flags_test_ZS(&cpu->flags, cpu->reg_L);
            flags_test_P(&cpu->flags, cpu->reg_L);
            break;
        case 0x06: //RLC (HL)
            instruction_rlc(cpu, &PTR(HL));
            flags_test_ZS(&cpu->flags, PTR(HL));
            flags_test_P(&cpu->flags, PTR(HL));
            break;
        case 0x07: //RLC A
            instruction_rlc(cpu, &cpu->reg_A);
            flags_test_ZS(&cpu->flags, cpu->reg_A);
            flags_test_P(&cpu->flags, cpu->reg_A);
            break;
        case 0x08: //RRC B
            instruction_rrc(cpu, &cpu->reg_B);
            flags_test_ZS(&cpu->flags, cpu->reg_B);
            flags_test_P(&cpu->flags, cpu->reg_B);
            break;
        case 0x09: //RRC C
            instruction_rrc(cpu, &cpu->reg_C);
            flags_test_ZS(&cpu->flags, cpu->reg_C);
            flags_test_P(&cpu->flags, cpu->reg_C);
            break;
        case 0x0a: //RRC D
            instruction_rrc(cpu, &cpu->reg_D);
            flags_test_ZS(&cpu->flags, cpu->reg_D);
            flags_test_P(&cpu->flags, cpu->reg_D);
            break;
        case 0x0b: //RRC E
            instruction_rrc(cpu, &cpu->reg_E);
            flags_test_ZS(&cpu->flags, cpu->reg_E);
            flags_test_P(&cpu->flags, cpu->reg_E);
            break;
        case 0x0c: //RRC H
            instruction_rrc(cpu, &cpu->reg_H);
            flags_test_ZS(&cpu->flags, cpu->reg_H);
            flags_test_P(&cpu->flags, cpu->reg_H);
            break;
        case 0x0d: //RRC L
            instruction_rrc(cpu, &cpu->reg_L);
            flags_test_ZS(&cpu->flags, cpu->reg_L);
            flags_test_P(&cpu->flags, cpu->reg_L);
            break;
        case 0x0e: //RRC (HL)
            instruction_rrc(cpu, &PTR(HL));
            flags_test_ZS(&cpu->flags, PTR(HL));
            flags_test_P(&cpu->flags, PTR(HL));
            break;
        case 0x0f: //RRC A
            instruction_rrc(cpu, &cpu->reg_A);
            flags_test_ZS(&cpu->flags, cpu->reg_A);
            flags_test_P(&cpu->flags, cpu->reg_A);
            break;
        case 0x10: //RL B
            instruction_rl(cpu, &cpu->reg_B);
            flags_test_ZS(&cpu->flags, cpu->reg_B);
            flags_test_P(&cpu->flags, cpu->reg_B);
            break;
        case 0x11: //RL C
            instruction_rl(cpu, &cpu->reg_C);
            flags_test_ZS(&cpu->flags, cpu->reg_C);
            flags_test_P(&cpu->flags, cpu->reg_C);
            break;
        case 0x12: //RL D
            instruction_rl(cpu, &cpu->reg_D);
            flags_test_ZS(&cpu->flags, cpu->reg_D);
            flags_test_P(&cpu->flags, cpu->reg_D);
            break;
        case 0x13: //RL E
            instruction_rl(cpu, &cpu->reg_E);
            flags_test_ZS(&cpu->flags, cpu->reg_E);
            flags_test_P(&cpu->flags, cpu->reg_E);
            break;
        case 0x14: //RL H
            instruction_rl(cpu, &cpu->reg_H);
            flags_test_ZS(&cpu->flags, cpu->reg_H);
            flags_test_P(&cpu->flags, cpu->reg_H);
            break;
        case 0x15: //RL L
            instruction_rl(cpu, &cpu->reg_L);
            flags_test_ZS(&cpu->flags, cpu->reg_L);
            flags_test_P(&cpu->flags, cpu->reg_L);
            break;
        case 0x16: //RL (HL)
            instruction_rl(cpu, &PTR(HL));
            flags_test_ZS(&cpu->flags, PTR(HL));
            flags_test_P(&cpu->flags, PTR(HL));
            break;
        case 0x17: //RL A
            instruction_rl(cpu, &cpu->reg_A);
            flags_test_ZS(&cpu->flags, cpu->reg_A);
            flags_test_P(&cpu->flags, cpu->reg_A);
            break;
        case 0x18: //RR B
            instruction_rr(cpu, &cpu->reg_B);
            flags_test_ZS(&cpu->flags, cpu->reg_B);
            flags_test_P(&cpu->flags, cpu->reg_B);
            break;
        case 0x19: //RR C
            instruction_rr(cpu, &cpu->reg_C);
            flags_test_ZS(&cpu->flags, cpu->reg_C);
            flags_test_P(&cpu->flags, cpu->reg_C);
            break;
        case 0x1a: //RR D
            instruction_rr(cpu, &cpu->reg_D);
            flags_test_ZS(&cpu->flags, cpu->reg_D);
            flags_test_P(&cpu->flags, cpu->reg_D);
            break;
        case 0x1b: //RR E
            instruction_rr(cpu, &cpu->reg_E);
            flags_test_ZS(&cpu->flags, cpu->reg_E);
            flags_test_P(&cpu->flags, cpu->reg_E);
            break;
        case 0x1c: //RR H
            instruction_rr(cpu, &cpu->reg_H);
            flags_test_ZS(&cpu->flags, cpu->reg_H);
            flags_test_P(&cpu->flags, cpu->reg_H);
            break;
        case 0x1d: //RR L
            instruction_rr(cpu, &cpu->reg_L);
            flags_test_ZS(&cpu->flags, cpu->reg_L);
            flags_test_P(&cpu->flags, cpu->reg_L);
            break;
        case 0x1e: //RR (HL)
            instruction_rr(cpu, &PTR(HL));
            flags_test_ZS(&cpu->flags, PTR(HL));
            flags_test_P(&cpu->flags, PTR(HL));
            break;
        case 0x1f: //RR A
            instruction_rr(cpu, &cpu->reg_A);
            flags_test_ZS(&cpu->flags, cpu->reg_A);
            flags_test_P(&cpu->flags, cpu->reg_A);
            break;
        case 0x20: //SLA B
            instruction_sla(cpu, &cpu->reg_B);
            break;
        case 0x21: //SLA C
            instruction_sla(cpu, &cpu->reg_C);
            break;
        case 0x22: //SLA D
            instruction_sla(cpu, &cpu->reg_D);
            break;
        case 0x23: //SLA E
            instruction_sla(cpu, &cpu->reg_E);
            break;
        case 0x24: //SLA H
            instruction_sla(cpu, &cpu->reg_H);
            break;
        case 0x25: //SLA L
            instruction_sla(cpu, &cpu->reg_L);
            break;
        case 0x26: //SLA (HL)
            instruction_sla(cpu, &PTR(HL));
            break;
        case 0x27: //SLA A
            instruction_sla(cpu, &cpu->reg_A);
            break;
        case 0x28: //SRA B
            instruction_sra(cpu, &cpu->reg_B);
            break;
        case 0x29: //SRA C
            instruction_sra(cpu, &cpu->reg_C);
            break;
        case 0x2a: //SRA D
            instruction_sra(cpu, &cpu->reg_D);
            break;
        case 0x2b: //SRA E
            instruction_sra(cpu, &cpu->reg_E);
            break;
        case 0x2c: //SRA H
            instruction_sra(cpu, &cpu->reg_H);
            break;
        case 0x2d: //SRA L
            instruction_sra(cpu, &cpu->reg_L);
            break;
        case 0x2e: //SRA (HL)
            instruction_sra(cpu, &PTR(HL));
            break;
        case 0x2f: //SRA A
            instruction_sra(cpu, &cpu->reg_A);
            break;
        case 0x38: //SRL B
            instruction_srl(cpu, &cpu->reg_B);
            break;
        case 0x39: //SRL C
            instruction_srl(cpu, &cpu->reg_C);
            break;
        case 0x3a: //SRL D
            instruction_srl(cpu, &cpu->reg_D);
            break;
        case 0x3b: //SRL E
            instruction_srl(cpu, &cpu->reg_E);
            break;
        case 0x3c: //SRL H
            instruction_srl(cpu, &cpu->reg_H);
            break;
        case 0x3d: //SRL L
            instruction_srl(cpu, &cpu->reg_L);
            break;
        case 0x3e: //SRL (HL)
            instruction_srl(cpu, &PTR(HL));
            break;
        case 0x3f: //SRL A
            instruction_srl(cpu, &cpu->reg_A);
            break;
        case 0x40 ... 0x7f: //BIT byte, REG
            instruction_bit(cpu, *opcode);
            break;
        case 0x80 ... 0xbf: //RES byte, REG
            instruction_res_set(cpu, *opcode, 0);
            break;
        case 0xc0 ... 0xff: //SET byte, REG
            instruction_res_set(cpu, *opcode, 1);
            break;
    }
}

void instruction_IXIY_add(Flags *flags, uint16_t *ix_or_iy, uint16_t reg_pair){
    //TODO: Check PC increment in all prefix instructions
    //TODO: Fix this instruction and send it to instructions.c
    uint32_t result = *ix_or_iy + reg_pair;
    flags->cy = result & 0x10000? 1: 0;
    flags->n = 0;
    flags_test_H16(flags, *ix_or_iy, reg_pair, 0);
    *ix_or_iy = result; 
}

void cpu_IXIY_instructions(Cpu8080 *cpu, uint8_t *opcode, bool iy_mode){
    uint16_t *ix_or_iy = iy_mode? &cpu->reg_IY : &cpu->reg_IX;
    switch(*opcode){
        case 0x09: //ADD IX/Y,BC
            instruction_IXIY_add(&cpu->flags, ix_or_iy, read_reg_BC(cpu));
            break;
        case 0x19: //ADD IX/Y,DE
            instruction_IXIY_add(&cpu->flags, ix_or_iy, read_reg_DE(cpu));
            break;
        case 0x21: //LD IX/Y,nn
            *ix_or_iy = cpu_GetLIWord(cpu);
            break;
        case 0x22: //LD (nn),IX/Y
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            cpu->memory->memory[adr] = *ix_or_iy;
            break;
            }
        case 0x23: //INC IX/Y
            (*ix_or_iy) ++;
            break;
        case 0x29: //ADD IX/Y,IX/Y
            instruction_IXIY_add(&cpu->flags, ix_or_iy, *ix_or_iy);
            break;
        case 0x2a: //LD IX/Y,(nn)
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            *ix_or_iy = cpu->memory->memory[adr];
            break;
            }
        case 0x2b: //DEC IX/Y
            (*ix_or_iy) --;
            break;
        case 0x34: //INC (IX/Y+d)
            {
            uint16_t adr = *ix_or_iy + memory_read8(cpu->memory, ++cpu->PC);
            cpu->memory->memory[adr] ++;
            break;
            }
        case 0x35: //DEC (IX/Y+d)
            {
            uint16_t adr = *ix_or_iy + memory_read8(cpu->memory, ++cpu->PC);
            cpu->memory->memory[adr] --;
            break;
            }
        case 0x36: //LD (IX/Y+d),n
            {
            uint16_t adr = *ix_or_iy + memory_read8(cpu->memory, ++cpu->PC);
            cpu->memory->memory[adr] = memory_read8(cpu->memory, ++cpu->PC);
            break;
            }
        case 0x39: //ADD IX/Y,SP
            instruction_IXIY_add(&cpu->flags, ix_or_iy, cpu->SP);
            break;
        case 0x40 ... 0x7f: //LD reg1,reg2 
            instruction_ld_IXIY(cpu, *opcode, iy_mode);
            break;
        case 0x86: //ADD A,(IX/Y+d)
            {
            uint16_t adr = *ix_or_iy + memory_read8(cpu->memory, ++cpu->PC);
            instruction_add(cpu, cpu->memory->memory[adr]); 
            break;
            }
        case 0x96: //SUB A,(IX/Y+d)
            {
            uint16_t adr = *ix_or_iy + memory_read8(cpu->memory, ++cpu->PC);
            instruction_sub(cpu, cpu->memory->memory[adr]); 
            break;
            }
        case 0x9e: //SBC A,(IX/Y+d)
            {
            uint16_t adr = *ix_or_iy + memory_read8(cpu->memory, ++cpu->PC);
            instruction_sbc(cpu, cpu->memory->memory[adr]); 
            break;
            }
        case 0xa6: //AND A,(IX/Y+d)
            {
            uint16_t adr = *ix_or_iy + memory_read8(cpu->memory, ++cpu->PC);
            instruction_ana(cpu, cpu->memory->memory[adr]); 
            break;
            }
        case 0xae: //XOR A,(IX/Y+d)
            {
            uint16_t adr = *ix_or_iy + memory_read8(cpu->memory, ++cpu->PC);
            instruction_xra(cpu, cpu->memory->memory[adr]); 
            break;
            }
        case 0xb6: //OR A,(IX/Y+d)
            {
            uint16_t adr = *ix_or_iy + memory_read8(cpu->memory, ++cpu->PC);
            instruction_ora(cpu, cpu->memory->memory[adr]); 
            break;
            }
        case 0xbe: //XOR A,(IX/Y+d)
            {
            uint16_t adr = *ix_or_iy + memory_read8(cpu->memory, ++cpu->PC);
            instruction_cmp(cpu, cpu->memory->memory[adr]); 
            break;
            }
        case 0xcb: //PREFIX: IX/Y BIT INSTRUCTIONS
            break;
        case 0xe1: //POP IX/Y
            (*ix_or_iy) = stack_pop16(cpu);
            break;
        case 0xe3: //EX (SP),IX/Y
            {
            uint16_t temp = cpu->SP;
            cpu->SP = (*ix_or_iy);
            (*ix_or_iy) = temp;
            break;
            }
        case 0xe5: //PUSH IX/Y
            stack_push16(cpu, (*ix_or_iy));
            break;
        case 0xe9: //JP (IX/Y)
            cpu->PC = (*ix_or_iy) - 1;
            break;
        case 0xf9: //LD SP,IX/Y
            cpu->SP = (*ix_or_iy);
            break;
    }
}

void cpu_IXIY_bit_instructions(Cpu8080 *cpu, uint8_t opcode, bool iy_mode){
    uint16_t *ix_or_iy = iy_mode? &cpu->reg_IY : &cpu->reg_IX;
    uint16_t adr = (*ix_or_iy) + memory_read8(cpu->memory, ++cpu->PC);
    switch(opcode){
        case 0x06: //RLC (IX/Y+d)
            instruction_rlc(cpu, &cpu->memory->memory[adr]);
            break;
        case 0x0e: //RRC (IX/Y+d)
            instruction_rrc(cpu, &cpu->memory->memory[adr]);
            break;
        case 0x16: //RL (IX/Y+d)
            instruction_rl(cpu, &cpu->memory->memory[adr]);
            break;
        case 0x1e: //RR (IX/Y+d)
            instruction_rr(cpu, &cpu->memory->memory[adr]);
            break;
        case 0x26: //SLA (IX/Y+d)
            instruction_sla(cpu, &cpu->memory->memory[adr]);
            break;
        case 0x2e: //SRA (IX/Y+d)
            instruction_sra(cpu, &cpu->memory->memory[adr]);
            break;
        case 0x3e: //SRL (IX/Y+d)
            instruction_srl(cpu, &cpu->memory->memory[adr]);
            break;
        case 0x40 ... 0x7f: //BIT x,(IX/Y + d)
            instruction_bit_IXIY(cpu, opcode, (*ix_or_iy));
            break;
        case 0x80 ... 0xbf: //RES x,(IX/Y + d)
            instruction_res_set_IXIY(cpu, opcode, 0, adr);
            break;
        case 0xc0 ... 0xff: //RES x,(IX/Y + d)
            instruction_res_set_IXIY(cpu, opcode, 1, adr);
            break;
    }
}

void instruction_misc_adc(struct cpu8080 *cpu, uint16_t reg_x){
    uint32_t result = read_reg_HL(cpu) + reg_x + cpu->flags.cy;
    //NOTE: if this doesnt pass tests, try adding the borrow to cpu->reg_A instead
    flags_test_V16(&cpu->flags, read_reg_HL(cpu), reg_x + cpu->flags.cy);
    flags_test_H16(&cpu->flags, read_reg_HL(cpu), reg_x, cpu->flags.cy);
    flags_test_C16(&cpu->flags, result);
    cpu->flags.z = result == 0? 1 : 0;
    cpu->flags.s = result & 0x8000? 1 : 0;
    cpu->flags.n = 0;

    write_reg_HL(cpu, result);
}

void instruction_misc_sbc(struct cpu8080 *cpu, uint16_t reg_x){
    uint8_t borrow = ~(cpu->flags.cy) & 0x01;
    uint32_t result = read_reg_HL(cpu) + ~reg_x + borrow;

    cpu->flags.cy = 0;
    if((result ^ read_reg_HL(cpu) ^ ~reg_x) & 0x0100){ 
        cpu->flags.cy = 1;
    }

    //NOTE: if this doesnt pass tests, try adding the borrow to cpu->reg_A instead
    flags_test_V16(&cpu->flags, read_reg_HL(cpu), ~(reg_x) + borrow);
    flags_test_H16(&cpu->flags, read_reg_HL(cpu), ~reg_x, borrow);
    cpu->flags.z = result == 0? 1 : 0;
    cpu->flags.s = result & 0x8000? 1 : 0;
    cpu->flags.cy = ~cpu->flags.cy & 0x01;
    cpu->flags.n = 1;

    write_reg_HL(cpu, result);
}


void cpu_misc_instructions(Cpu8080 *cpu, uint8_t opcode){
    switch(opcode){
        case 0x40: //IN B,(c)
            io_routines_IN(cpu, &cpu->reg_B);
            break;
        case 0x41: //OUT (c),B
            io_routines_OUT(cpu, &cpu->reg_B);
            break;
        case 0x42: //SBC HL,BC
            instruction_misc_sbc(cpu, read_reg_BC(cpu));
            break;
        case 0x43: //LD (nn),BC
            break;
        case 0x44: //NEG
            break;
        case 0x45: //RETN
            break;
        case 0x46: //IM 0
            break;
        case 0x47: //LD I,A
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
            break;
        case 0x4d: //RETI
            break;
        case 0x4f: //LD R,A
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
            break;
        case 0x56: //IM 1
            break;
        case 0x57: //LD A,I
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
            break;
        case 0x5e: //IM 2
            break;
        case 0x5f: //LD A,R
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
            break;
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
            break;
        case 0x72: //SBC HL,SP
            instruction_misc_sbc(cpu, cpu->SP);
            break;
        case 0x73: //LD (nn),SP
            break;
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
            break;
        case 0xa0: //LDI
            break;
        case 0xa1: //CPI
            break;
        case 0xa2: //INI
            break;
        case 0xa3: //OUTI
            break;
        case 0xa8: //LDD
            break;
        case 0xa9: //CPD
            break;
        case 0xaa: //IND
            break;
        case 0xab: //OUTD
            break;
        case 0xb0: //LDIR
            break;
        case 0xb1: //CPIR
            break;
        case 0xb2: //INIR
            break;
        case 0xb3: //OTIR 
            break;
        case 0xb8: //LDDR
            break;
        case 0xb9: //CPDR
            break;
        case 0xba: //INDR
            break;
        case 0xbb: //OTDR
            break;
    }
}
