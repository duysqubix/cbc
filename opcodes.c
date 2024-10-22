#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "opcodes.h"
#include "gameboy.h"



// 0x00 NOP
opcycle_t OP_NOP(GAMEBOY *gb, void *value) {
    gb->pc++;
    return OPCYCLE;
}

// 0x10 STOP CPU & >CD display until putton pressed
opcycle_t OP_STOP(GAMEBOY *gb, void *value) {
    if (gb->cgb_mode) {
        printf("STOP CPU & DISPLAY\n");
    }

    gb->pc++;
    gb->pc++;
    return OPCYCLE;
}

//0x50 LD D, B
opcycle_t OP_LD(GAMEBOY *gb, void *value) {
    gb->d = gb->b;
    gb->pc++;
    return OPCYCLE;
}

opcode_func opcodes[256] = {
    [0x00] = OP_NOP,
    [0x50] = OP_LD
};

opcycle_t execute_opcode(GAMEBOY *gb, opcode_t opcode, void *opcode_value) {
    opcycle_t cycles = 0;

    if (opcodes[opcode] != NULL) {
        cycles = opcodes[opcode](gb, opcode_value);
    } else {
        printf("Error: Opcode %02X not implemented\n", opcode);
        exit(1);
    }

    return cycles;
}



