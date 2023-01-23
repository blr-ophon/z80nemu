#ifndef CPU_H
#define CPU_H 

#define PTR(HL)  cpu->memory->memory[read_reg_HL(cpu)]

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "./stack.h"
#include "./memory.h"
#include "./flags.h"
#include "./registerbank.h"
#include "./instructions.h"
#include "./config.h"

typedef struct cpu8080{
    Memory *memory;
    Flags flags;

    //Registers
    uint8_t reg_A;
    uint8_t reg_B;
    uint8_t reg_C;
    uint8_t reg_D;
    uint8_t reg_E;
    uint8_t reg_H;
    uint8_t reg_L;

    uint16_t PC;
    uint16_t SP;
    uint16_t reg_IX;
    uint16_t reg_IY;
    uint16_t reg_INTRV;

    uint8_t data_bus;
    uint16_t address_bus;


    //Pins
    bool RD_pin:1;    //active low
    bool WR_pin:1;    //active low
    bool IORQ_pin:1;  //active low
    bool MREQ_pin:1;  //active low
    bool INT_pin:1;   //active low
    bool NMI_pin:1;   //active low

    //Internal Registers
    bool interrupt_enable:1;
    uint32_t step_count;
} Cpu8080;

uint16_t cpu_GetLIWord(Cpu8080 *cpu);

void cpu_init(Cpu8080 *cpu, Memory *memory);

void cpu_fetch_execute(Cpu8080 *cpu);

void cpu_exec_instruction(Cpu8080 *cpu, uint8_t *opcode);

void cpu_bit_instructions(Cpu8080 *cpu, uint8_t *opcode);

#endif
