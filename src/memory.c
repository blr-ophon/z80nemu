#include "memory.h" 
#include <assert.h>

static void check_boundaries(Memory *memory, uint16_t address){
    assert(address <= memory->size);
}

Memory *memory_init(size_t size){
    Memory *memory = malloc(sizeof(Memory));
    memory->memory = calloc(size, sizeof(uint8_t));
    memory->size = size;
    return memory;
}

void memory_free(Memory *memory){
    free(memory->memory);
    free(memory);
}

//concatenate all to memory_read and number of bytes as a parameter
//makes no sense since there is a limit to number of bytes that can
//be read (8)
uint8_t memory_read8(Memory *memory, uint16_t address){
    //create in cpu get_next_bytes(size) which increments PC after reading
    check_boundaries(memory, address);
    return memory->memory[address];
}

uint16_t memory_read16(Memory *memory, uint16_t address){
    check_boundaries(memory, address);
    uint16_t value = memory->memory[address];
    value += memory->memory[address + 1] << 8;
    return value;
}

void memory_write8(Memory *memory, uint16_t address, uint8_t value){
//    check_RAM_boundaries(memory, address); //cant write to ROM
    memory->memory[address] = value;
}

void memory_write16(Memory *memory, uint16_t address, uint16_t value){
 //   check_RAM_boundaries(address); //cant write to ROM
    memory->memory[address] = (uint8_t) value & 0x00ff;
    memory->memory[address+1] = (uint8_t)  value >> 8;
}

void memory_clear(Memory *memory){
    memset(memory->memory, 0, sizeof(memory->size));
}

void memory_program_load(Memory *memory, uint8_t *buf, size_t size, uint16_t adr){
    void *memPtr = memory->memory + adr;
    memcpy(memPtr, buf, size);
    //assert buf not greater than memory
    //memory_dump(memory, size);
}

void memory_dump(Memory *memory, long size){
    for(int i = 0; i < size/16 ; i++){
        printf("%#06x  ", 16*i);
        for(int j = 0; j < 16; j++){
            printf("%02x ", memory->memory[16*i + j]);
        }
        printf("\n");
    }
}
