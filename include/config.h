#ifndef CONFIG_H
#define CONFIG_H

//TODO: Add more cpu constants

#define INTERRUPT_VECTOR_SIZE 8

//Error codes
#define EINVPREFIX  1

////////////////////////////////////////////////////////////////////////////////
//                  Indexes used in instruction decoding
////////////////////////////////////////////////////////////////////////////////
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

#define ROT_RLC_IDX         0
#define ROT_RRC_IDX         1
#define ROT_RL_IDX          2
#define ROT_RR_IDX          3
#define ROT_SLA_IDX         4
#define ROT_SRA_IDX         5
#define ROT_SLL_IDX         6
#define ROT_SRL_IDX         7
////////////////////////////////////////////////////////////////////////////////

#endif
