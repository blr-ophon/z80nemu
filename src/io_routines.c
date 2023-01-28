#include "io_routines.h"
#include "cpu.h"

//TODO: document the process of programmed IO, Interrupt and DMA here
//
//NOTE: even though IN and OUT use BC as address, the IO only uses the lower
//byte (c) as address, giving it 256 possibilities. In hardware, this means the 
//upper 8 address pins are not connected at all to IO chips.

void io_routines_IN(struct cpu8080 *cpu, uint8_t *reg_x){
    //Instruction is executed in two parts. 
    //
    //In the first one it sends the address to address bus and sets proper 
    //pins, then breaks opcode execution so that a machine specific function
    //can do whatever it wants with this information, such as strobing or,
    //what is more common, set a byte in address bus to be read.
    //
    //In the second one, it resets the proper pins and read whatever
    //is in data bus to an specified register, setting the flags accordingly.
    //
    //
    //FIRST PART OF INSTRUCTION
    if(cpu->IORQ_pin){ //active low
        cpu->IORQ_pin = 0;
        cpu->MREQ_pin = 1;
        cpu->RD_pin = 0;
        cpu->WR_pin = 1;
        *(cpu->address_bus) = read_reg_BC(cpu);
        cpu->PC -= 2; //TODO: check/test
        return;
        //this function will be called again for the second part as long as 
        //nothing changes PC. TODO: Protect this routine against interrupts 
        //and DMAs. If an interrupt changes PC in the middle of an instruction,
        //this can break the emulation because of the pins. The emulation will
        //fetch instruction again for IN instructions, but the actual hardware
        //only fetches IN one time, so the second time does not count as an 
        //actual fetch to allow DMA and interrupts.
    } 

    //SECOND PART OF INSTRUCTION
    //pins
    cpu->IORQ_pin = 1;
    cpu->MREQ_pin = 0;
    //flags
    *reg_x = *(cpu->data_bus);
    cpu->flags.n = 0;
    flags_test_P(&cpu->flags, *reg_x);
    cpu->flags.h = 0;
    flags_test_ZS(&cpu->flags, *reg_x);
}

void io_routines_OUT(struct cpu8080 *cpu, uint8_t *reg_x){
    //Instruction is executed in two parts. 
    //
    //In the first one, it sets the proper flags and latches the byte in
    //the data bus, then breaks opcode execution so that a machine specific
    //instruction can do whatever it wants with the data, either that or just
    //be strobed by the pins.
    //
    //In the second one, it resets the pins and return program execution.
    //
    //FIRST PART OF INSTRUCTION
    if(cpu->IORQ_pin){ //active low
        cpu->IORQ_pin = 0;
        cpu->MREQ_pin = 1;
        cpu->RD_pin = 1;
        cpu->WR_pin = 0;
        *(cpu->address_bus) = read_reg_BC(cpu);
        *(cpu->data_bus) = *reg_x;
        cpu->PC -= 2; //TODO: check/test
        return;
        //this function will be called again for the second part as long as 
        //nothing changes PC. TODO: Protect this routine against interrupts 
        //and DMAs. If an interrupt changes PC in the middle of an instruction,
        //this can break the emulation because of the pins. The emulation will
        //fetch instruction again for IN instructions, but the actual hardware
        //only fetches IN one time, so the second time does not count as an 
        //actual fetch to allow DMA and interrupts. Create a pin M.
    } 

    //SECOND PART OF INSTRUCTION
    //pins
    cpu->IORQ_pin = 1;
    cpu->MREQ_pin = 0;
    //flags
        cpu->flags.n = 0;
    flags_test_P(&cpu->flags, *reg_x);
    cpu->flags.h = 0;
    flags_test_ZS(&cpu->flags, *reg_x);
}
