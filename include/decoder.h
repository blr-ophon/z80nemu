#ifndef DECODER_H
#define DECODER_H

#include "cpu.h"

#define EINVPREFIX  1

typedef enum{
    ADMODE_IMMEDIATE    = 0,   //Opcode, imm
    ADMODE_IMMEDIATE_EX = 0,   //Opcode, Limm, Himm
}AdressingMode;

typedef struct{
    int AdressingMode;
    uint16_t prefix;
    uint8_t opcode;
    uint8_t displacement;
    uint8_t immediate;
}DCD_Instruction;

int DCD_FetchDecode(Cpuz80 *cpu, DCD_Instruction *instruction);
int DCD_isPrefix(uint8_t prefix_byte);



#endif
