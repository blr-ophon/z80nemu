#include "test.h"

int main(int argc, char *argv[]){
    printf("\n");
    const char *filename;
    if(argc < 2){
        fprintf(stderr, "Must provide a file\n");
        return -1;
    }else{
        filename = argv[1];
        fprintf(stdin, " Loading file: %s\n", filename);
    }
    FILE *f = fopen(filename, "rb");
    if(!f){
        fprintf(stderr, "Failed to read from file\n");
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    uint8_t buf[size];
    int res = fread(buf, 1, size, f);
    if(!res){
        fprintf(stderr, "Failed to read from file\n");
        exit(1);
    }

    Memory *memory = memory_init(0x4000);
    memory_clear(memory);
    memory_program_load(memory, buf, size,  0x0100);
    fclose(f);

    Cpu8080 cpu;
    cpu_init(&cpu, memory);
    cpu.PC = 0x0100;

    while(1){
        if(cpu.PC == 0x0000){
            printf("\nCall 0\n");
            exit(EXIT_SUCCESS);
        }
        if(cpu.PC == 0x0005){
            test_print_subroutine(&cpu);
        }
        cpu_cycle(&cpu);
    }

    memory_free(memory);
    return 0;
}

void test_print_subroutine(Cpu8080 *cpu){
    //execute subroutine
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
    }

    //return
    cpu->PC = stack_pop16(cpu);
}

