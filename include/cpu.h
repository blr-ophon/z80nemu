#ifndef CPU_H
#define CPU_H 

#define PTR_DE  cpu->memory->memory[read_reg_DE(cpu)]
#define PTR_HL  cpu->memory->memory[read_reg_HL(cpu)]

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "stack.h"
#include "memory.h"
#include "flags.h"
#include "registerbank.h"
#include "instructions.h"
#include "config.h"
#include "io_routines.h"

//TODO:Check that, in the program, all pins that "go out" of the cpu are
//only set/reset by the cpu emulation, while all pins that "come in" to the
//cpu are set/reset by (emulated) external devices.

typedef struct cpuz80{
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
    uint8_t reg_I;
    uint8_t reg_R;
    uint16_t PC;
    uint16_t SP;
    uint16_t reg_IX;
    uint16_t reg_IY;
    uint16_t reg_INTRV;
    //Pins
    bool M_pin:1;       //active low / modified by cpu
    bool RD_pin:1;      //active low / modified by cpu
    bool WR_pin:1;      //active low / modified by cpu
    bool IORQ_pin:1;    //active low / modified by cpu
    bool MREQ_pin:1;    //active low / modified by cpu
    bool INT_pin:1;     //active low / modified by external device
    bool NMI_pin:1;     //active low / modified by external device
    //Internal Registers
    bool interrupt_enable:1;
    uint32_t step_count;
    //Pointers to bus
    uint8_t *data_bus;
    uint16_t *address_bus;
} Cpuz80;

//PINS AND BUSES BEHAVIOR:
//RD,WR pins and address/data buses will not be set properly in me-
//-mory reads/writes, as it's not necessary to emulate the instructions.
//The pins and buses will only work as intended during IO interactions.
//
//M1 PIN BEHAVIOR:
//As the emulator only check for pins imediatly before instruction
//fetch, there is no need to change the M1 pin during execution, as
//it's expected to be always 0 at that time. The only exception are
//IN/OUT instrcutions, which are executed in two parts, with pin and
//bus modifications between them. M1 is set to 1 during those to in-
//dicate that the CPU is still executing something and can't be in-
//terrupted.

uint16_t cpu_GetLIWord(Cpuz80 *cpu);

void cpu_init(Cpuz80 *cpu, Memory *memory);

void cpu_cycle(Cpuz80 *cpu);

void cpu_exec_instruction(Cpuz80 *cpu, uint8_t *opcode);

void cpu_bit_instructions(Cpuz80 *cpu, uint8_t *opcode);

void cpu_IXIY_instructions(Cpuz80 *cpu, uint8_t *opcode, bool iy_mode);

void cpu_IXIY_bit_instructions(Cpuz80 *cpu, uint8_t *opcode, uint8_t d_operand, bool iy_mode);

void cpu_misc_instructions(Cpuz80 *cpu, uint8_t opcode);

#endif
