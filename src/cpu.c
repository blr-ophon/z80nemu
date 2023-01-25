#include "cpu.h"

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

void cpu_fetch_execute(Cpu8080 *cpu){

    if(cpu->INT_pin && cpu->interrupt_enable){ //RST, byte
        cpu->INT_pin = 0;
        stack_push16(cpu, cpu->PC);
//        cpu->PC = INTERRUPT_VECTOR_SIZE * cpu->interrupt_address;
    }

    uint8_t opcode = memory_read8(cpu->memory, cpu->PC);
    cpu_exec_instruction(cpu, &opcode);
    cpu->PC ++;
    cpu->step_count++;
}

void cpu_exec_instruction(Cpu8080 *cpu, uint8_t *opcode){
    //TODO: Check proper CPU Flags handling
    switch(*opcode){
        case 0x00: //NOP
            break;
        case 0x01: //LD BC, word 
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
        case 0x06: //MVI B, byte 
            cpu->reg_B = memory_read8(cpu->memory, ++cpu->PC);
            break;
        case 0x07: //RLCA
            instruction_rlc(cpu, &cpu->reg_A);
            break;
        case 0x09: //ADD HL, BC
            {
            cpu->flags.n = 0;
            uint16_t word1 = read_reg_HL(cpu);
            uint16_t word2 = read_reg_BC(cpu);
            flags_test_C16(&cpu->flags, (uint32_t) word1 + word2);
            word1 += word2;
            write_reg_HL(cpu, word1);
            break;
            }
        case 0x0a: //LDAX B 
            {
            uint16_t adr = read_reg_BC(cpu);
            cpu->reg_A = cpu->memory->memory[adr];
            break;
            }
        case 0x0b: //DCX B 
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
        case 0x0e: //MVI C, byte 
            cpu->reg_C = memory_read8(cpu->memory, ++cpu->PC);
            break;
        case 0x0f: //RRCA
            instruction_rrc(cpu, &cpu->reg_A);
            break;
        case 0x11: //LD DE, word 
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
        case 0x13: //INX D 
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
        case 0x16: //MVI D, byte 
            cpu->reg_D = memory_read8(cpu->memory, ++cpu->PC);
            break;
        case 0x17: //RLA
            instruction_rl(cpu, &cpu->reg_A);
            break;
        case 0x19: //ADD HL, DE
            {
            cpu->flags.n = 0;
            uint16_t word1 = read_reg_HL(cpu);
            uint16_t word2 = read_reg_DE(cpu);
            flags_test_C16(&cpu->flags, (uint32_t) word1 + word2);
            word1 += word2;
            write_reg_HL(cpu, word1);
            break;
            }
        case 0x1a: //LDAX D 
            {
            uint16_t adr = read_reg_DE(cpu);
            cpu->reg_A = cpu->memory->memory[adr];
            break;
            }
        case 0x1b: //DCX D 
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
        case 0x1e: //MVI E, byte 
            cpu->reg_E = memory_read8(cpu->memory, ++cpu->PC);
            break;
        case 0x1f: //RRA
            instruction_rr(cpu, &cpu->reg_A);
            break;
        case 0x21: //LD HL, word 
            {
            uint16_t word = cpu_GetLIWord(cpu);
            write_reg_HL(cpu, word);
            break;
            }
        case 0x22: //SHLD adr  
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            cpu->memory->memory[adr] = cpu->reg_L;
            cpu->memory->memory[adr+1] = cpu->reg_H;
            break;
            }
        case 0x23: //INX H 
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
        case 0x26: //MVI H, byte 
            cpu->reg_H = memory_read8(cpu->memory, ++cpu->PC);
            break;
        case 0x27: //DAA 
            {
            bool cy = cpu->flags.cy;
            uint8_t correction = 0;

            uint8_t ls_nibble = cpu->reg_A & 0x0f;
            if((ls_nibble > 0x09) || cpu->flags.ac){
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
        case 0x29: //ADD HL, HL
            {
            cpu->flags.n = 0;
            uint16_t word1 = read_reg_HL(cpu);
            uint16_t word2 = read_reg_HL(cpu);
            flags_test_C16(&cpu->flags, (uint32_t) word1 + word2);
            word1 += word2;
            write_reg_HL(cpu, word1);
            break;
            }
        case 0x2a: //LHLD adr 
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            cpu->reg_L = cpu->memory->memory[adr];
            cpu->reg_H = cpu->memory->memory[adr+1];
            break;
            }
        case 0x2b: //DCX H
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
        case 0x2e: //MVI L, byte
            cpu->reg_L = memory_read8(cpu->memory, ++cpu->PC);
            break;
        case 0x2f: //CPL
            cpu->flags.n = 1;
            cpu->reg_A = ~cpu->reg_A;
            break;
        case 0x31: //LD SP, word 
            {
            uint16_t word = cpu_GetLIWord(cpu);
            cpu->SP = word;
            break;
            }
        case 0x32: //STA adr 
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            cpu->memory->memory[adr] = cpu->reg_A;
            break;
            }
        case 0x33: //INX SP
            cpu->SP ++;
            break;
        case 0x34: //INC M 
            instruction_inc_8(cpu, &PTR(HL), 1);
            break;
        case 0x35: //DEC M
            instruction_inc_8(cpu, &PTR(HL), -1);
            break;
        case 0x36: //MVI M,byte
            cpu->memory->memory[read_reg_HL(cpu)] = memory_read8(cpu->memory, ++cpu->PC);
            break;
        case 0x37: //SCF
            cpu->flags.n = 0;
            cpu->flags.cy = 1;
            break;
        case 0x39: //ADD HL, SP
            {
            cpu->flags.n = 0;
            uint16_t word = read_reg_HL(cpu);
            flags_test_C16(&cpu->flags, (uint32_t) word + cpu->SP);
            word += cpu->SP;
            write_reg_HL(cpu, word);
            break;
            }
        case 0x3a: //LDA adr 
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            cpu->reg_A = cpu->memory->memory[adr];
            break;
            }
        case 0x3b: //DCX SP 
            cpu->SP --;
            break;
        case 0x3c: //INC A
            instruction_inc_8(cpu, &cpu->reg_A, 1);
            break;
        case 0x3d: //DEC A
            instruction_inc_8(cpu, &cpu->reg_A, -1);
            break;
        case 0x3e: //MVI A,byte
            cpu->reg_A = memory_read8(cpu->memory, ++cpu->PC);
            break;
        case 0x3f: //CCF
            cpu->flags.n = 0;
            cpu->flags.cy = ~cpu->flags.cy;
            break;
        case 0x40: //MOV B,B 
            cpu->reg_B = cpu->reg_B;
            break;
        case 0x41: //MOV B,C
            cpu->reg_B = cpu->reg_C;
            break;
        case 0x42: //MOV B,D 
            cpu->reg_B = cpu->reg_D;
            break;
        case 0x43: //MOV B,E 
            cpu->reg_B = cpu->reg_E;
            break;
        case 0x44: //MOV B,H 
            cpu->reg_B = cpu->reg_H;
            break;
        case 0x45: //MOV B,L 
            cpu->reg_B = cpu->reg_L;
            break;
        case 0x46: //MOV B,M 
            cpu->reg_B = PTR(HL);
            break;
        case 0x47: //MOV B,A 
            cpu->reg_B = cpu->reg_A;
            break;
        case 0x48: //MOV C,B 
            cpu->reg_C = cpu->reg_B;
            break;
        case 0x49: //MOV C,C 
            cpu->reg_C = cpu->reg_C;
            break;
        case 0x4a: //MOV C,D 
            cpu->reg_C = cpu->reg_D;
            break;
        case 0x4b: //MOV C,E 
            cpu->reg_C = cpu->reg_E;
            break;
        case 0x4c: //MOV C,H 
            cpu->reg_C = cpu->reg_H;
            break;
        case 0x4d: //MOV C,L
            cpu->reg_C = cpu->reg_L;
            break;
        case 0x4e: //MOV C,M
            cpu->reg_C = PTR(HL);
            break;
        case 0x4f: //MOV C,A 
            cpu->reg_C = cpu->reg_A;
            break;
        case 0x50: //MOV D,B 
            cpu->reg_D = cpu->reg_B;
            break;
        case 0x51: //MOV D,C 
            cpu->reg_D = cpu->reg_C;
            break;
        case 0x52: //MOV D,D 
            cpu->reg_D = cpu->reg_D;
            break;
        case 0x53: //MOV D,E 
            cpu->reg_D = cpu->reg_E;
            break;
        case 0x54: //MOV D,H 
            cpu->reg_D = cpu->reg_H;
            break;
        case 0x55: //MOV D,L 
            cpu->reg_D = cpu->reg_L;
            break;
        case 0x56: //MOV D,M
            cpu->reg_D = PTR(HL);
            break;
        case 0x57: //MOV D,A 
            cpu->reg_D = cpu->reg_A;
            break;
        case 0x58: //MOV E,B 
            cpu->reg_E = cpu->reg_B;
            break;
        case 0x59: //MOV E,C 
            cpu->reg_E = cpu->reg_C;
            break;
        case 0x5a: //MOV E,D 
            cpu->reg_E = cpu->reg_D;
            break;
        case 0x5b: //MOV E,E 
            cpu->reg_E = cpu->reg_E;
            break;
        case 0x5c: //MOV E,H 
            cpu->reg_E = cpu->reg_H;
            break;
        case 0x5d: //MOV E,L 
            cpu->reg_E = cpu->reg_L;
            break;
        case 0x5e: //MOV E,M 
            cpu->reg_E = PTR(HL);
            break;
        case 0x5f: //MOV E,A 
            cpu->reg_E = cpu->reg_A;
            break;
        case 0x60: //MOV H,B 
            cpu->reg_H = cpu->reg_B;
            break;
        case 0x61: //MOV H,C
            cpu->reg_H = cpu->reg_C;
            break;
        case 0x62: //MOV H,D
            cpu->reg_H = cpu->reg_D;
            break;
        case 0x63: //MOV H,E
            cpu->reg_H = cpu->reg_E;
            break;
        case 0x64: //MOV H,H
            cpu->reg_H = cpu->reg_H;
            break;
        case 0x65: //MOV H,L
            cpu->reg_H = cpu->reg_L;
            break;
        case 0x66: //MOV H,M
            cpu->reg_H = PTR(HL);
            break;
        case 0x67: //MOV H,A
            cpu->reg_H = cpu->reg_A;
            break;
        case 0x68: //MOV L,B
            cpu->reg_L = cpu->reg_B;
            break;
        case 0x69: //MOV L,C 
            cpu->reg_L = cpu->reg_C;
            break;
        case 0x6a: //MOV L,D 
            cpu->reg_L = cpu->reg_D;
            break;
        case 0x6b: //MOV L,E 
            cpu->reg_L = cpu->reg_E;
            break;
        case 0x6c: //MOV L,H
            cpu->reg_L = cpu->reg_H;
            break;
        case 0x6d: //MOV L,L
            cpu->reg_L = cpu->reg_L;
            break;
        case 0x6e: //MOV L,M
            cpu->reg_L = PTR(HL);
            break;
        case 0x6f: //MOV L,A
            cpu->reg_L = cpu->reg_A;
            break;
        case 0x70: //MOV M,B
            cpu->memory->memory[read_reg_HL(cpu)] = cpu->reg_B;
            break;
        case 0x71: //MOV M,C 
            cpu->memory->memory[read_reg_HL(cpu)] = cpu->reg_C;
            break;
        case 0x72: //MOV M,D 
            cpu->memory->memory[read_reg_HL(cpu)] = cpu->reg_D;
            break;
        case 0x73: //MOV M,E
            cpu->memory->memory[read_reg_HL(cpu)] = cpu->reg_E;
            break;
        case 0x74: //MOV M,H
            cpu->memory->memory[read_reg_HL(cpu)] = cpu->reg_H;
            break;
        case 0x75: //MOV M,L 
            cpu->memory->memory[read_reg_HL(cpu)] = cpu->reg_L;
            break;
        case 0x77: //MOV M,A 
            cpu->memory->memory[read_reg_HL(cpu)] = cpu->reg_A;
            break;
        case 0x78: //MOV A,B 
            cpu->reg_A = cpu->reg_B;
            break;
        case 0x79: //MOV A,C 
            cpu->reg_A = cpu->reg_C;
            break;
        case 0x7a: //MOV A,D 
            cpu->reg_A = cpu->reg_D;
            break;
        case 0x7b: //MOV A,E 
            cpu->reg_A = cpu->reg_E;
            break;
        case 0x7c: //MOV A,H
            cpu->reg_A = cpu->reg_H;
            break;
        case 0x7d: //MOV A,L
            cpu->reg_A = cpu->reg_L;
            break;
        case 0x7e: //MOV A,M
            cpu->reg_A = PTR(HL);
            break;
        case 0x7f: //MOV A,A
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
        case 0x8e: //ADC A,M
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
        case 0x96: //SUB A,M
            instruction_sub(cpu, PTR(HL));
            break;
        case 0x97: //SUB A,A
            instruction_sub(cpu, cpu->reg_A);
            break;
        case 0x98: //SBC A,B
            instruction_sbb(cpu, cpu->reg_B);
            break;
        case 0x99: //SBC A,C
            instruction_sbb(cpu, cpu->reg_C);
            break;
        case 0x9a: //SBC A,D
            instruction_sbb(cpu, cpu->reg_D);
            break;
        case 0x9b: //SBC A,E
            instruction_sbb(cpu, cpu->reg_E);
            break;
        case 0x9c: //SBC A,H
            instruction_sbb(cpu, cpu->reg_H);
            break;
        case 0x9d: //SBC A,L
            instruction_sbb(cpu, cpu->reg_L);
            break;
        case 0x9e: //SBC A,M
            instruction_sbb(cpu, PTR(HL));
            break;
        case 0x9f: //SBC A,A
            instruction_sbb(cpu, cpu->reg_A);
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
        case 0xa6: //AND M
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
        case 0xae: //XOR M
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
        case 0xb6: //ORA M
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
        case 0xbe: //CMP M
            instruction_cmp(cpu, PTR(HL));
            break;
        case 0xbf: //CMP A
            instruction_cmp(cpu, cpu->reg_A);
            break;
        case 0xc0: //RNZ
            if(!cpu->flags.z){
                cpu->PC = stack_pop16(cpu) -1;
            }
            break;
        case 0xc1: //POP B
            {
            uint16_t word = stack_pop16(cpu);
            write_reg_BC(cpu, word);
            break;
            }
        case 0xc2: //JNZ adr
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(!cpu->flags.z){
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xc3: //JMP adr
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            cpu->PC = adr -1;
            break;
            }
        case 0xc4: //CNZ adr
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(!cpu->flags.z){
                stack_push16(cpu, cpu->PC+1);
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xc5: //PUSH B
            {
            uint16_t word = read_reg_BC(cpu);
            stack_push16(cpu, word);
            break;
            }
        case 0xc6: //ADI byte 
            {
            uint8_t operand = memory_read8(cpu->memory, ++cpu->PC);
            instruction_add(cpu, operand);
            break;
            }
        case 0xc7: //RST 0
            stack_push16(cpu, cpu->PC+1);
            cpu->PC = (INTERRUPT_VECTOR_SIZE * 0) - 1;
            break;
        case 0xc8: //RZ
            if(cpu->flags.z){
                cpu->PC = stack_pop16(cpu) -1;
            }
            break;
        case 0xc9: //RET
            cpu->PC = stack_pop16(cpu) -1;
            break;
        case 0xca: //JZ adr
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
        case 0xcc: //CZ
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(cpu->flags.z){
                stack_push16(cpu, cpu->PC+1);
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xcd: //CALL adr
            {
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
        case 0xce: //ACI byte
            {
            uint8_t operand = memory_read8(cpu->memory, ++cpu->PC); 
            instruction_adc(cpu, operand);
            break;
            }
        case 0xcf: //RST 1
            stack_push16(cpu, cpu->PC+1);
            cpu->PC = (INTERRUPT_VECTOR_SIZE * 1) - 1;
            break;
        case 0xd0: //RNC
            if(!cpu->flags.cy){
                cpu->PC = stack_pop16(cpu) -1;
            }
            break;
        case 0xd1: //POP D
            {
            uint16_t word = stack_pop16(cpu);
            write_reg_DE(cpu, word);
            break;
            }
        case 0xd2: //JNC adr
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(!cpu->flags.cy){
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xd3: //OUT byte 
            {
            //duplicated byte on address bus
            uint16_t adr = (uint16_t) memory_read8(cpu->memory, ++cpu->PC);
            adr |= adr << 8;
            cpu->address_bus = adr; 
            //reg_A on data_bus
            cpu->data_bus = cpu->reg_A;
            cpu->IORQ_pin = 0;  //active low 
            cpu->WR_pin = 0;    //active low 
            cpu->RD_pin = 1;    //active low
            break;
            }
        case 0xd4: //CNC adr
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(!cpu->flags.cy){
                stack_push16(cpu, cpu->PC+1);
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xd5: //PUSH D
            stack_push16(cpu, read_reg_DE(cpu));
            break;
        case 0xd6: //SUI byte 
            {
            uint8_t operand = memory_read8(cpu->memory, ++cpu->PC); 
            instruction_sub(cpu, operand);
            break;
            }
        case 0xd7: //RST 2;
            stack_push16(cpu, cpu->PC+1);
            cpu->PC = (INTERRUPT_VECTOR_SIZE * 2) - 1;
            break;
        case 0xd8: //RC
            if(cpu->flags.cy){
                cpu->PC = stack_pop16(cpu) -1;
            }
            break;
        case 0xda: //JC adr
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(cpu->flags.cy){
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xdb: //IN byte
            {
            uint16_t adr = (uint16_t) memory_read8(cpu->memory, ++cpu->PC);
            adr |= adr << 8;
            cpu->address_bus = adr; 
            cpu->IORQ_pin = 0;  //active low 
            cpu->WR_pin = 1;    //active low 
            cpu->RD_pin = 0;    //active low
            break;
            }
        case 0xdc: //CC adr
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
            cpu_IX_instructions(cpu, &opcode);
            break;
            }
        case 0xde: //SBI byte 
            {
            uint8_t operand = memory_read8(cpu->memory, ++cpu->PC); 
            instruction_sbb(cpu, operand);
            break;
            }
        case 0xdf: //RST 3;
            stack_push16(cpu, cpu->PC+1);
            cpu->PC = (INTERRUPT_VECTOR_SIZE * 3) - 1;
            break;
        case 0xe0: //RPO
            if(!cpu->flags.p){
                cpu->PC = stack_pop16(cpu) -1;
            }
            break;
        case 0xe1: //POP H
            {
            uint16_t word = stack_pop16(cpu);
            write_reg_HL(cpu, word);
            break;
            }
        case 0xe2: //JPO adr
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(!cpu->flags.p){
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xe3: //XTHL 
            {
            uint16_t word = stack_pop16(cpu);
            stack_push16(cpu, read_reg_HL(cpu));
            write_reg_HL(cpu, word);
            break;
            }
        case 0xe4: //CPO adr
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(!cpu->flags.p){
                stack_push16(cpu, cpu->PC+1);
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xe5: //PUSH H
            stack_push16(cpu, read_reg_HL(cpu));
            break;
        case 0xe6: //ANI byte 
            {
            uint8_t operand = memory_read8(cpu->memory, ++cpu->PC); 
            instruction_ana(cpu, operand);
            break;
            }
        case 0xe7: //RST 4;
            stack_push16(cpu, cpu->PC+1);
            cpu->PC = (INTERRUPT_VECTOR_SIZE * 4) - 1;
            break;
        case 0xe8: //RPE
            if(cpu->flags.p){
                cpu->PC = stack_pop16(cpu) -1;
            }
            break;
        case 0xe9: //PCHL
            {
            uint16_t word = read_reg_HL(cpu);
            cpu->PC = word -1;
            break;
            }
        case 0xea: //JPE adr
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(cpu->flags.p){
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xeb: //XCHG
            {
            uint16_t temp = read_reg_HL(cpu);
            write_reg_HL(cpu, read_reg_DE(cpu));
            write_reg_DE(cpu, temp);
            break;
            }
        case 0xec: //CPE adr
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(cpu->flags.p){
                stack_push16(cpu, cpu->PC+1);
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xee: //XRI byte 
            {
            uint8_t operand = memory_read8(cpu->memory, ++cpu->PC); 
            instruction_xra(cpu, operand);
            break;
            }
        case 0xef: //RST 5;
            stack_push16(cpu, cpu->PC+1);
            cpu->PC = (INTERRUPT_VECTOR_SIZE * 5) - 1;
            break;
        case 0xf0: //RP
            if(!cpu->flags.s){
                cpu->PC = stack_pop16(cpu) -1;
            }
            break;
        case 0xf1: //POP PSW
            {
            uint8_t flags_reg = stack_pop8(cpu);
            flags_sta_byte(&cpu->flags, flags_reg);
            cpu->reg_A = stack_pop8(cpu);
            break;
            }
        case 0xf2: //JP
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
        case 0xf4: //CP adr
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(!cpu->flags.s){
                stack_push16(cpu, cpu->PC+1);
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xf5: //PUSH PSW
            {
            uint8_t flags_reg = flags_load_byte(&cpu->flags);
            stack_push8(cpu, cpu->reg_A);
            stack_push8(cpu, flags_reg);
            break;
            }
        case 0xf6: //ORI byte 
            {
            uint8_t operand = memory_read8(cpu->memory, ++cpu->PC); 
            instruction_ora(cpu, operand);
            break;
            }
        case 0xf7: //RST 6
            stack_push16(cpu, cpu->PC+1);
            cpu->PC = (INTERRUPT_VECTOR_SIZE * 6) - 1;
            break;
        case 0xf8: //RM
            if(cpu->flags.s){
                cpu->PC = stack_pop16(cpu) -1;
            }
            break;
        case 0xf9: //SPHL
            {
            uint16_t word = read_reg_HL(cpu);
            cpu->SP = word;
            break;
            }
        case 0xfa: //JM adr
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
        case 0xfc: //CM adr
            {
            uint16_t adr = cpu_GetLIWord(cpu);
            if(cpu->flags.s){
                stack_push16(cpu, cpu->PC+1);
                cpu->PC = adr -1;
            }
            break;
            }
        case 0xfe: //CPI byte 
            {
            uint8_t operand = memory_read8(cpu->memory, ++cpu->PC);
            instruction_cmp(cpu, operand);
            break;
            }
        case 0xff: //RST 7
            stack_push16(cpu, cpu->PC + 1);
            cpu->PC = (INTERRUPT_VECTOR_SIZE * 7) - 1;
            break;
        default:
            unimplemented_opcode(cpu,opcode);
            break;
    }
}

void cpu_bit_instructions(Cpu8080 *cpu, uint8_t *opcode){
    switch(*opcode){
        //TODO: Check proper CPU Flags handling
        case 0x00: //RLC B
            instruction_rlc(cpu, &cpu->reg_B);
            flags_test_ZS(&cpu->flags, cpu->reg_B);
            break;
        case 0x01: //RLC C
            instruction_rlc(cpu, &cpu->reg_C);
            flags_test_ZS(&cpu->flags, cpu->reg_C);
            break;
        case 0x02: //RLC D
            instruction_rlc(cpu, &cpu->reg_D);
            flags_test_ZS(&cpu->flags, cpu->reg_D);
            break;
        case 0x03: //RLC E
            instruction_rlc(cpu, &cpu->reg_E);
            flags_test_ZS(&cpu->flags, cpu->reg_E);
            break;
        case 0x04: //RLC H
            instruction_rlc(cpu, &cpu->reg_H);
            flags_test_ZS(&cpu->flags, cpu->reg_H);
            break;
        case 0x05: //RLC L
            instruction_rlc(cpu, &cpu->reg_L);
            flags_test_ZS(&cpu->flags, cpu->reg_L);
            break;
        case 0x06: //RLC (HL)
            instruction_rlc(cpu, &PTR(HL));
            flags_test_ZS(&cpu->flags, PTR(HL));
            break;
        case 0x07: //RLC A
            instruction_rlc(cpu, &cpu->reg_A);
            flags_test_ZS(&cpu->flags, cpu->reg_A);
            break;
        case 0x08: //RRC B
            instruction_rrc(cpu, &cpu->reg_B);
            flags_test_ZS(&cpu->flags, cpu->reg_B);
            break;
        case 0x09: //RRC C
            instruction_rrc(cpu, &cpu->reg_C);
            flags_test_ZS(&cpu->flags, cpu->reg_C);
            break;
        case 0x0a: //RRC D
            instruction_rrc(cpu, &cpu->reg_D);
            flags_test_ZS(&cpu->flags, cpu->reg_D);
            break;
        case 0x0b: //RRC E
            instruction_rrc(cpu, &cpu->reg_E);
            flags_test_ZS(&cpu->flags, cpu->reg_E);
            break;
        case 0x0c: //RRC H
            instruction_rrc(cpu, &cpu->reg_H);
            flags_test_ZS(&cpu->flags, cpu->reg_H);
            break;
        case 0x0d: //RRC L
            instruction_rrc(cpu, &cpu->reg_L);
            flags_test_ZS(&cpu->flags, cpu->reg_L);
            break;
        case 0x0e: //RRC (HL)
            instruction_rrc(cpu, &PTR(HL));
            flags_test_ZS(&cpu->flags, PTR(HL));
            break;
        case 0x0f: //RRC A
            instruction_rrc(cpu, &cpu->reg_A);
            flags_test_ZS(&cpu->flags, cpu->reg_A);
            break;
        case 0x10: //RL B
            instruction_rl(cpu, &cpu->reg_B);
            flags_test_ZS(&cpu->flags, cpu->reg_B);
            break;
        case 0x11: //RL C
            instruction_rl(cpu, &cpu->reg_C);
            flags_test_ZS(&cpu->flags, cpu->reg_C);
            break;
        case 0x12: //RL D
            instruction_rl(cpu, &cpu->reg_D);
            flags_test_ZS(&cpu->flags, cpu->reg_D);
            break;
        case 0x13: //RL E
            instruction_rl(cpu, &cpu->reg_E);
            flags_test_ZS(&cpu->flags, cpu->reg_E);
            break;
        case 0x14: //RL H
            instruction_rl(cpu, &cpu->reg_H);
            flags_test_ZS(&cpu->flags, cpu->reg_H);
            break;
        case 0x15: //RL L
            instruction_rl(cpu, &cpu->reg_L);
            flags_test_ZS(&cpu->flags, cpu->reg_L);
            break;
        case 0x16: //RL (HL)
            instruction_rl(cpu, &PTR(HL));
            flags_test_ZS(&cpu->flags, PTR(HL));
            break;
        case 0x17: //RL A
            instruction_rl(cpu, &cpu->reg_A);
            flags_test_ZS(&cpu->flags, cpu->reg_A);
            break;
        case 0x18: //RR B
            instruction_rr(cpu, &cpu->reg_B);
            flags_test_ZS(&cpu->flags, cpu->reg_B);
            break;
        case 0x19: //RR C
            instruction_rr(cpu, &cpu->reg_C);
            flags_test_ZS(&cpu->flags, cpu->reg_C);
            break;
        case 0x1a: //RR D
            instruction_rr(cpu, &cpu->reg_D);
            flags_test_ZS(&cpu->flags, cpu->reg_D);
            break;
        case 0x1b: //RR E
            instruction_rr(cpu, &cpu->reg_E);
            flags_test_ZS(&cpu->flags, cpu->reg_E);
            break;
        case 0x1c: //RR H
            instruction_rr(cpu, &cpu->reg_H);
            flags_test_ZS(&cpu->flags, cpu->reg_H);
            break;
        case 0x1d: //RR L
            instruction_rr(cpu, &cpu->reg_L);
            flags_test_ZS(&cpu->flags, cpu->reg_L);
            break;
        case 0x1e: //RR (HL)
            instruction_rr(cpu, &PTR(HL));
            flags_test_ZS(&cpu->flags, PTR(HL));
            break;
        case 0x1f: //RR A
            instruction_rr(cpu, &cpu->reg_A);
            flags_test_ZS(&cpu->flags, cpu->reg_A);
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

void cpu_IX_instructions(Cpu8080 *cpu, uint8_t *opcode){
    switch(*opcode){
        case 0x09: //ADD IX,BC
            {
            uint16_t operand = read_reg_BC(cpu);
            uint32_t result = cpu->reg_IX + operand;
            if(result & 0x10000){
                cpu->flags.cy = 1;
            }else cpu->flags.cy = 0;

            if(result & 0x8000){
                cpu->flags.n = 1;
            }else cpu->flags.n = 0;

            break;
            }
        case 0x19: //ADD IX,DE
            break;

    }
}

