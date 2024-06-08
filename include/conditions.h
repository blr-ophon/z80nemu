#ifndef CONDITIONS_H
#define CONDITIONS_H

#include "cpu.h"
#include <stdbool.h>

bool condition_TRUE(struct cpuz80 *cpu);
bool condition_DNZ(struct cpuz80 *cpu);
bool condition_NZ(struct cpuz80 *cpu);
bool condition_Z(struct cpuz80 *cpu);
bool condition_NC(struct cpuz80 *cpu);
bool condition_C(struct cpuz80 *cpu);
bool condition_PO(struct cpuz80 *cpu);
bool condition_PE(struct cpuz80 *cpu);
bool condition_P(struct cpuz80 *cpu);
bool condition_M(struct cpuz80 *cpu);

#endif
