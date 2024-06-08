#include "conditions.h"

bool condition_TRUE(struct cpuz80 *cpu){
    return true;
}

bool condition_FALSE(struct cpuz80 *cpu){
    return true;
}

bool condition_DNZ(struct cpuz80 *cpu){
    cpu->reg_B--;
    return (cpu->reg_B != 0);
}


bool condition_NZ(struct cpuz80 *cpu){
    return !cpu->flags.z;
}

bool condition_Z(struct cpuz80 *cpu){
    return cpu->flags.z;
}

bool condition_NC(struct cpuz80 *cpu){
    return !cpu->flags.cy;
}

bool condition_C(struct cpuz80 *cpu){
    return cpu->flags.cy;
}

bool condition_PO(struct cpuz80 *cpu){
    return !cpu->flags.p;
}

bool condition_PE(struct cpuz80 *cpu){
    return cpu->flags.p;
}

bool condition_P(struct cpuz80 *cpu){
    return !cpu->flags.s;
}

bool condition_M(struct cpuz80 *cpu){
    return cpu->flags.s;
}
