#include "../include/opcodelog.h" 

uint8_t used_opcodes[0xff] = {0xff}; 
//TODO: Implement this in other way, 
//this won't work if RST 7 (0xff) is executed

void bubble_sort(uint8_t array[]){
    //TODO: implement qsort please...
    for(int i = 0; i < 0xff; i++){
        for(int j = 0; j < 0xff - 1; j++){
            if(array[j+1] < array[j]){
                uint8_t temp = array[j];
                array[j] = array[j+1];
                array[j+1] = temp;
            }
        }

    }
}

void insert_opcode(uint8_t opcode){
    int i = 0;
    while(used_opcodes[i] != 0xff){
        if(used_opcodes[i] == opcode){
            return;
        }
        i++;
    }
    used_opcodes[i] = opcode;
}

void create_log_file(void){
    bubble_sort(used_opcodes);
    FILE *f;
    f = fopen("erroropcodes.dat", "w");
    for(int i = 0; used_opcodes[i] != 0xff; i++){
        fprintf(f, "%03d:  %02x\n", i, used_opcodes[i]);
    }
    fclose(f);
}
