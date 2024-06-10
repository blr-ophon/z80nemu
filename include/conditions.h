#ifndef CONDITIONS_H
#define CONDITIONS_H

#include "z80.h"

bool condition_TRUE(Cpuz80 *cpu);
bool condition_DNZ(Cpuz80 *cpu);
bool condition_NZ(Cpuz80 *cpu);
bool condition_Z(Cpuz80 *cpu);
bool condition_NC(Cpuz80 *cpu);
bool condition_C(Cpuz80 *cpu);
bool condition_PO(Cpuz80 *cpu);
bool condition_PE(Cpuz80 *cpu);
bool condition_P(Cpuz80 *cpu);
bool condition_M(Cpuz80 *cpu);

#endif
