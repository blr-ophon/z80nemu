#ifndef DECODER_H
#define DECODER_H

#include "cpu.h"

#define EINVPREFIX  1



//Indexes used in instruction decoding

#define R_B_IDX          0
#define R_C_IDX          1
#define R_D_IDX          2
#define R_E_IDX          3
#define R_H_IDX          4
#define R_L_IDX          5
#define R_pHL_IDX        6
#define R_A_IDX          7

#define RP1_BC_IDX          0
#define RP1_DE_IDX          1
#define RP1_HL_IDX          2
#define RP1_SP_IDX          3

#define RP2_BC_IDX          0
#define RP2_DE_IDX          1
#define RP2_HL_IDX          2
#define RP2_AF_IDX          3

#define CC_NZ_IDX           0
#define CC_Z_IDX            1
#define CC_NC_IDX           2
#define CC_C_IDX            3
#define CC_PO_IDX           4
#define CC_PE_IDX           5
#define CC_P_IDX            6
#define CC_M_IDX            7



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
