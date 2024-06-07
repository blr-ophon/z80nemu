#include "decoder.h"


/*
 * Assert prefix byte is valid
 */
int DCD_isPrefix(uint8_t prefix_byte){
    uint8_t rv = 0;
    switch(prefix_byte){
        case 0xcb:
        case 0xdd:
        case 0xed:
        case 0xfd:
            rv = 1;
            break;
        default:
            rv = 0;
            break;
    }
    return rv;
}


/*
 * Fetch and Decode next instruction
 * Check if instruction is valid and set flags if it isn't
 */
int DCD_FetchDecode(Cpuz80 *cpu, DCD_Instruction *inst){
    int rv = 0;


    //First fetch: prefix
    uint8_t byteValue = memory_read8(cpu->memory, cpu->PC++);
    while(DCD_isPrefix(byteValue)){
        inst->prefix <<= 8;
        inst->prefix |= byteValue;
        byteValue = memory_read8(cpu->memory, cpu->PC++);
        //TODO: this might be wrong, check prefix endianness
    }

    if(inst->prefix > 0xff){ //if inst has double prefix
        inst->displacement = byteValue;
        inst->opcode = memory_read8(cpu->memory, cpu->PC++);
    }else{
        inst->opcode = byteValue;
        //TODO: decode opcode and discover its instruction format
    }

    return rv;
}

