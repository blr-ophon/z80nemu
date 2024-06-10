#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
//#include "./config.h"

////////////////////////////////////////////////////////////////////////////////
/// MEMORY MAP
///
/// 0000-1FFF 8k ROM
/// 2000-23FF 1k RAM
/// 2400-3FFF 7k Video RAM
/// 4000- RAM mirror
///
/// *little endian: lsB on lower address
/// Ex.: c3 $18d4 :  c3 d4 18
///
////////////////////////////////////////////////////////////////////////////////


//MAKE SIZE OF MEOMORY VARIABLE FOR REUSE OF THIS PROJECT
//CHECK HL POINTING TO M IN CPU.C
//EITHER THAT OR IMPLEMENTS MASKS/SHADOWS
typedef struct memory{
    uint8_t *memory;
    size_t size;
}Memory;

Memory *memory_init(size_t size);

void memory_free(Memory *memory);

uint8_t memory_read8(Memory *memory, uint16_t address);

uint16_t memory_read16(Memory *memory, uint16_t address);

void memory_write8(Memory *memory, uint16_t address, uint8_t value);

void memory_write16(Memory *memory, uint16_t address, uint16_t value);

void memory_clear(Memory *memory);

void memory_program_load(Memory *memory, uint8_t *buf, size_t size, uint16_t adr);

void memory_dump(Memory *memory, long size);

#endif
