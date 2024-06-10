#include "cpu.h"

//TODO: create a common header for cpu.h, flags.h, instructions.h and io_routines.h
//to avoid redefinition of structs errors
//TODO: CPU speed
//TODO: proper PC increment: save a copy of (pc), increment pc
//and execute the copy. this simulates the pc being saved in instruction
//register before being incremented. This should be done last, as it breaks
//byte fetchs, pushs, jumps, calls, in, out etc

static void unimplemented_opcode(Cpuz80 *cpu, uint8_t *opcode){
    fprintf(stderr, "Unimplemented Opcode: %02X\n", *opcode);
    fprintf(stderr, "Step: %d\n", cpu->step_count);
    exit(1);
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
    instr_main(cpu, opcode);
    cpu->PC ++;
    cpu->step_count++;

    //Check for interrupts
    if(!cpu->INT_pin && cpu->interrupt_enable && !cpu->M_pin){ 
        cpu_int_handler(cpu);
    }
    //Check for bus requests
}


static inline uint16_t GetLIWord(Cpuz80 *cpu){
    uint16_t word = cpu->memory->memory[++cpu->PC];
    word ^= (uint16_t) cpu->memory->memory[++cpu->PC] << 8;
    return word;
}

