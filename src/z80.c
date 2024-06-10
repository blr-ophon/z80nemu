#include "z80.h"

uint16_t z80_fetchLIWord(Cpuz80 *cpu){
    uint16_t word = cpu->memory->memory[++cpu->PC];
    word ^= (uint16_t) cpu->memory->memory[++cpu->PC] << 8;
    return word;
}



