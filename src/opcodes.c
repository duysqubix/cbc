
#include "defs.h"


static opcycles_t nop(){                            // 0x00
    return MCYCLE_1;
}

static opcycles_t ld_bc_nn(){                       // 0x01
    uint16_t value = READ_NEXT_WORD();
    REG_B = U8(value >> 8);
    REG_C = U8(value & 0xFF);
    return MCYCLE_3;
}

static opcycles_t ld_mem_bc_a(){     // 0x02
    WRITE_MEM(BC(), REG_A);
    return MCYCLE_2;
}

static opcycles_t inc_bc(){         // 0x03
    INC_BC();
    return MCYCLE_1;
}

static opcycles_t inc_b(){          // 0x04
    REG_B++;
    return MCYCLE_1;
}

static opcycles_t dec_b(){          // 0x05
    REG_B--;
    return MCYCLE_1;
}

static opcycles_t ld_b_n(){         // 0x06
    REG_B = READ_NEXT_BYTE();
    return MCYCLE_2;
}

static opcycles_t rlca(){          // 0x07
    FLAG_Z_RESET();
    FLAG_N_RESET();
    FLAG_H_RESET();
    FLAG_C_RESET();

    if BIT_IS_SET(REG_A, 7){
        FLAG_C_SET();
        REG_A = (REG_A << 1) + 1;
    } else {
        REG_A <<= 1;
    }

    return MCYCLE_1;
}

static opcycles_t ld_mem_nn_sp(){     // 0x08
    uint16_t value = READ_NEXT_WORD();
    WRITE_MEM(U16(value), U8(REG_SP & 0xFF));
    WRITE_MEM(U16(value+1), U8(REG_SP >> 8));
    return MCYCLE_5;
}

static opcycles_t add_hl_bc(){        // 0x09
    ADD_SET_FLAGS16(HL(), BC());

    REG_H = U8((U16(HL()) + U16(BC())) >> 8);
    REG_L = U8((U16(HL()) + U16(BC())) & 0xFF);
    return MCYCLE_2;
}

static opcycles_t ld_a_mem_bc(){      // 0x0A
    REG_A = READ_MEM(BC());
    return MCYCLE_2;
}

static opcycles_t dec_bc(){          // 0x0B
    DEC_BC();
    return MCYCLE_2;
}

static opcycles_t inc_c(){          // 0x0C
    REG_C++;
    return MCYCLE_1;
}

static opcycles_t dec_c(){          // 0x0D
    REG_C--;
    return MCYCLE_1;
}

static opcycles_t ld_c_n(){          // 0x0E
    REG_C = READ_NEXT_BYTE();
    return MCYCLE_2;
}

static opcycles_t rrca(){          // 0x0F
    FLAG_Z_RESET();
    FLAG_N_RESET();
    FLAG_H_RESET();
    FLAG_C_RESET();

    if BIT_IS_SET(REG_A, 0){
        FLAG_C_SET();
        REG_A = (REG_A >> 1) + 0x80;
    } else {
        REG_A >>= 1;
    }

    return MCYCLE_1;
}

static opcycles_t ld_mem_de_nn(){     // 0x11
    uint16_t value = READ_NEXT_WORD();
    REG_D = U8(value >> 8);
    REG_E = U8(value & 0xFF);
    return MCYCLE_3;
}

static opcycles_t ld_hl_nn(){        // 0x21
    uint16_t value = READ_NEXT_WORD();
    REG_H = U8(value >> 8);
    REG_L = U8(value & 0xFF);
    return MCYCLE_3;
}

static opcycles_t ld_sp_nn(){        // 0x31
    REG_SP = READ_NEXT_WORD();
    return MCYCLE_3;
}

static opcycles_t ld_a_n(){          // 0x3E
    REG_A = READ_NEXT_BYTE();
    return MCYCLE_2;
}

static opcycles_t ld_a_b(){          // 0x78
    REG_A = REG_B;
    return MCYCLE_1;
}

static opcycles_t ld_a_c(){          // 0x79
    REG_A = REG_C;
    return MCYCLE_1;
}


static opcycles_t ld_a_h(){          // 0x7C
    REG_A = REG_H;
    return MCYCLE_1;
}

static opcycles_t jp_nz_nn(){          // 0xC2
        if (!FLAG_Z_IS_SET()){
            REG_PC = READ_NEXT_WORD()-3;
            return MCYCLE_4;
        }
        return MCYCLE_3;
}

static opcycles_t jp_nn(){          // 0xC3
    // Adjust PC to account for REG_PC being incremented on return
    REG_PC = READ_NEXT_WORD()-3;
    return MCYCLE_4;

}

static opcycles_t di(){             // 0xF3
    IE = 0x00;
    return MCYCLE_1;
}

static opcycles_t ld_hl_sp_i8(){   // 0xF8
    uint8_t value = READ_NEXT_BYTE();
    REG_H = U8((REG_SP + i8(value)) >> 8);
    REG_L = U8((REG_SP + i8(value)) & 0xFF);
    return MCYCLE_3;

}

static opcycles_t ld_sp_hl(){      // 0xF9
    REG_SP = (address_t)HL();
    return MCYCLE_2;
}

static opcycles_t ld_a_mem_nn(){      // 0xFA
    REG_A = READ_MEM(READ_NEXT_WORD());
    return MCYCLE_4;
}

static opcycles_t cp_n(){          // 0xFE
    CP_SET_FLAGS(REG_A, READ_NEXT_BYTE());
    return MCYCLE_2;
}


opcode_def_t *opcodes[512] = {
    nop,            ld_bc_nn,       ld_mem_bc_a,    inc_bc,         inc_b,          dec_b,          ld_b_n,         rlca,          // 0x00 - 0x07
    ld_mem_nn_sp,   add_hl_bc,      ld_a_mem_bc,    dec_bc,         inc_c,          dec_c,          ld_c_n,         rrca,          // 0x08 - 0x0F
    NULL,           ld_mem_de_nn,    NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x10 - 0x17
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x18 - 0x1F
    NULL,           ld_hl_nn,        NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x20 - 0x27
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x28 - 0x2F
    NULL,           ld_sp_nn,        NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x30 - 0x37
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x38 - 0x3F
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x40 - 0x47
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x48 - 0x4F
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x50 - 0x57
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x58 - 0x5F
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x60 - 0x67
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x68 - 0x6F
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x70 - 0x77
    ld_a_b,         ld_a_c,          NULL,           NULL,           ld_a_h,         NULL,           NULL,           NULL,          // 0x78 - 0x7F
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x80 - 0x87
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x88 - 0x8F
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x90 - 0x97
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x98 - 0x9F
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0xA0 - 0xA7
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0xA8 - 0xAF
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0xB0 - 0xB7
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0xB8 - 0xBF
    NULL,           NULL,            jp_nz_nn,       jp_nn,          NULL,           NULL,           NULL,           NULL,          // 0xC0 - 0xC7
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0xC8 - 0xCF
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0xD0 - 0xD7
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0xD8 - 0xDF
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0xE0 - 0xE7
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0xE8 - 0xEF
    NULL,           NULL,            NULL,           di,             NULL,           NULL,           NULL,           NULL,          // 0xF0 - 0xF7
    ld_hl_sp_i8,    ld_sp_hl,        ld_a_mem_nn,    NULL,           NULL,           NULL,           cp_n,           NULL,          // 0xF8 - 0xFF
    /****************************************************************************************************************/
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x100 - 0x107
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x108 - 0x10F
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x110 - 0x117
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x118 - 0x11F
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x120 - 0x127
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x128 - 0x12F
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x130 - 0x137
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x138 - 0x13F
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x140 - 0x147
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x148 - 0x14F
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x150 - 0x157
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x158 - 0x15F
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x160 - 0x167
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x168 - 0x16F
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x170 - 0x177
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x178 - 0x17F
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x180 - 0x187
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x188 - 0x18F
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x190 - 0x197
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x198 - 0x19F
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x1A0 - 0x1A7
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x1A8 - 0x1AF
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x1B0 - 0x1B7
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x1B8 - 0x1BF
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x1C0 - 0x1C7
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x1C8 - 0x1CF
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x1D0 - 0x1D7
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x1D8 - 0x1DF
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x1E0 - 0x1E7
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x1E8 - 0x1EF
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x1F0 - 0x1F7
    NULL,           NULL,            NULL,           NULL,           NULL,           NULL,           NULL,           NULL,          // 0x1F8 - 0x1FF
};