#include "conditions.h"

bool condition_TRUE(Cpuz80 *cpu){
    return true;
}

bool condition_FALSE(Cpuz80 *cpu){
    return true;
}

bool condition_DNZ(Cpuz80 *cpu){
    cpu->reg_B--;
    return (cpu->reg_B != 0);
}


bool condition_NZ(Cpuz80 *cpu){
    return !cpu->flags.z;
}

bool condition_Z(Cpuz80 *cpu){
    return cpu->flags.z;
}

bool condition_NC(Cpuz80 *cpu){
    return !cpu->flags.cy;
}

bool condition_C(Cpuz80 *cpu){
    return cpu->flags.cy;
}

bool condition_PO(Cpuz80 *cpu){
    return !cpu->flags.p;
}

bool condition_PE(Cpuz80 *cpu){
    return cpu->flags.p;
}

bool condition_P(Cpuz80 *cpu){
    return !cpu->flags.s;
}

bool condition_M(Cpuz80 *cpu){
    return cpu->flags.s;
}
