#include "conditions.h"

bool condition_TRUE(struct cpuz80 *cpu){
    return true;
}

bool condition_DNZ(struct cpuz80 *cpu){
    //TODO: check
    return (--cpu->reg_B != 0);
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
