#ifndef OPCODES_H
#define OPCODES_H

#include "gameboy.h"

#define MCYCLE_1 1
#define MCYCLE_2 2
#define MCYCLE_3 3
#define MCYCLE_4 4
#define MCYCLE_5 5
#define MCYCLE_6 6

typedef gbcycles_t opcode_def_t(Gameboy *gb);

extern opcode_def_t *opcodes[512];

#endif
