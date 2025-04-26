
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

static opcycles_t stop(){             // 0x10
    CPU_HALTED = true;
    CPU_STUCK = true;
    return MCYCLE_1;
}

static opcycles_t ld_mem_de_nn(){       // 0x11
    uint16_t value = READ_NEXT_WORD();
    REG_D = U8(value >> 8);
    REG_E = U8(value & 0xFF);
    return MCYCLE_3;
}

static opcycles_t ld_mem_de_a(){        // 0x12 
    WRITE_MEM(DE(), REG_A);
    return MCYCLE_2;
}

static opcycles_t inc_de(){             // 0x13
    INC_DE();
    return MCYCLE_1;
}

static opcycles_t inc_d(){              // 0x14
    REG_D++;
    return MCYCLE_1;
}

static opcycles_t dec_d(){              // 0x15
    REG_D--;
    return MCYCLE_1;
}

static opcycles_t ld_d_n(){             // 0x16
    REG_D = READ_NEXT_BYTE();
    return MCYCLE_2;
}

static opcycles_t rla(){              // 0x17
    FLAG_Z_RESET();
    FLAG_N_RESET();
    FLAG_H_RESET();

    uint8_t prev_carry = FLAG_C_IS_SET();

    if BIT_IS_SET(REG_A, 7){
        FLAG_C_SET();
    }else{
        FLAG_C_RESET();
    }

    REG_A = (REG_A << 1) & 0xff;

    if (prev_carry){
        REG_A |= 0x01;
    }

    return MCYCLE_1;
    
}

static opcycles_t jr_i8(){             // 0x18
    REG_PC += i8(READ_NEXT_BYTE())-2;
    return MCYCLE_3;
}

static opcycles_t add_hl_de(){         // 0x19
    ADD_SET_FLAGS16(HL(), DE());
    uint16_t result = HL() + DE();
    REG_H = U8(result >> 8);
    REG_L = U8(result & 0xFF);
    return MCYCLE_2;
}

static opcycles_t ld_a_mem_de(){       // 0x1A
    REG_A = READ_MEM(DE());
    return MCYCLE_2;
}

static opcycles_t dec_de(){             // 0x1B
    DEC_DE();
    return MCYCLE_1;
}

static opcycles_t inc_e(){              // 0x1C
    REG_E++;
    return MCYCLE_1;
}

static opcycles_t dec_e(){              // 0x1D
    REG_E--;
    return MCYCLE_1;
}


static opcycles_t ld_e_n(){             // 0x1E
    REG_E = READ_NEXT_BYTE();
    return MCYCLE_2;
}

static opcycles_t rra(){              // 0x1F
    FLAG_Z_RESET();
    FLAG_N_RESET();
    FLAG_H_RESET();

    uint8_t prev_carry = FLAG_C_IS_SET();

    if BIT_IS_SET(REG_A, 0){
        FLAG_C_SET();
    }else{
        FLAG_C_RESET();
    }

    REG_A = (REG_A >> 1) & 0xFF;

    if (prev_carry){
        REG_A |= 0x80;
    }

    return MCYCLE_1;
    
}

static opcycles_t jr_nz_i8(){         // 0x20
    if (!FLAG_Z_IS_SET()){
        REG_PC += i8(READ_NEXT_BYTE())-2;
        return MCYCLE_3;
    }
    return MCYCLE_2;
}


static opcycles_t ld_hl_nn(){           // 0x21
    uint16_t value = READ_NEXT_WORD();
    REG_H = U8(value >> 8);
    REG_L = U8(value & 0xFF);
    return MCYCLE_3;
}


static opcycles_t ld_hlp_a(){            // 0x22 
    WRITE_MEM(HL(), REG_A);
    INC_HL();
    return MCYCLE_2;
}


static opcycles_t daa(){              // 0x27
    uint16_t daa_result = U16(REG_A);

    FLAG_Z_RESET();

    if (FLAG_N_IS_SET()){
        if(FLAG_H_IS_SET()){
            daa_result = (daa_result - 0x06) & 0xff;
        }

        if(FLAG_C_IS_SET()){
            daa_result -= 0x60;
        }
    }else{
        if (FLAG_H_IS_SET() || (daa_result &0x0f) > 0x09){
            daa_result += 0x06;
        }

        if (FLAG_C_IS_SET() || daa_result > 0x9F){
            daa_result += 0x60;
        }
    }

    if((daa_result & 0xff) == 0 ){
        FLAG_Z_SET();
    }

    if((daa_result & 0x100) == 0x100){
        FLAG_C_SET();
    }

    FLAG_H_RESET();

    REG_A = U8(daa_result);

    return MCYCLE_2;    
}

static opcycles_t ld_sp_nn(){           // 0x31
    REG_SP = READ_NEXT_WORD();
    return MCYCLE_3;
}

static opcycles_t ld_mem_hl_n(){       // 0x36
    WRITE_MEM(HL(), READ_NEXT_BYTE());
    return MCYCLE_3;
}

static opcycles_t ld_a_n(){             // 0x3E
    REG_A = READ_NEXT_BYTE();
    return MCYCLE_2;
}

static opcycles_t ld_b_a(){             // 0x47
    REG_B = REG_A;
    return MCYCLE_1;
}

static opcycles_t ld_c_l(){             // 0x4D
    REG_C = REG_L;
    return MCYCLE_1;
}

static opcycles_t ld_d_h(){             //0x54 
    REG_D = REG_H;
    return MCYCLE_1;
}

static opcycles_t ld_e_l(){             // 0x5D
    REG_E = REG_L;
    return MCYCLE_1;
}

static opcycles_t ld_h_d(){             // 0x62
    REG_H = REG_D;
    return MCYCLE_1;
}


static opcycles_t ld_l_e(){             // 0x6B 
    REG_L = REG_E;
    return MCYCLE_1;
}

static opcycles_t ld_a_b(){             // 0x78
    REG_A = REG_B;
    return MCYCLE_1;
}

static opcycles_t ld_a_c(){             // 0x79
    REG_A = REG_C;
    return MCYCLE_1;
}

static opcycles_t ld_a_d(){             // 0x7A
    REG_A = REG_D;
    return MCYCLE_1;
}

static opcycles_t ld_a_e(){             // 0x7B
    REG_A = REG_E;
    return MCYCLE_1;
}

static opcycles_t ld_a_h(){             // 0x7C
    REG_A = REG_H;
    return MCYCLE_1;
}

static opcycles_t ld_a_l(){             // 0x7D
    REG_A = REG_L;
    return MCYCLE_1;
}

static opcycles_t jp_nz_nn(){          // 0xC2
        if (!FLAG_Z_IS_SET()){
            REG_PC = READ_NEXT_WORD()-3;
            return MCYCLE_4;
        }
        return MCYCLE_3;
}

static opcycles_t jp_nn(){              // 0xC3
    // Adjust PC to account for REG_PC being incremented on return
    REG_PC = READ_NEXT_WORD()-3;
    return MCYCLE_4;
}

static opcycles_t pop_hl(){             // 0xE1
    REG_H = READ_MEM(REG_SP+1);
    REG_L = READ_MEM(REG_SP);
    REG_SP += 2;
    return MCYCLE_3;
}

static opcycles_t push_hl(){             // 0xE5
    WRITE_MEM(REG_SP-1, REG_H);
    WRITE_MEM(REG_SP-2, REG_L);
    REG_SP -= 2;
    return MCYCLE_4;
}

static opcycles_t ld_mm_nn_a(){         // 0xEA
    WRITE_MEM(READ_NEXT_WORD(), REG_A);
    return MCYCLE_4;
}

static opcycles_t di(){                 // 0xF3
    IE = 0x00;
    return MCYCLE_1;
}

static opcycles_t pop_af(){             // 0xF1
    REG_A = READ_MEM(REG_SP+1);
    REG_F = READ_MEM(REG_SP);
    REG_SP += 2;
    return MCYCLE_3;
}

static opcycles_t push_af(){             // 0xF5
    WRITE_MEM(REG_SP-1, REG_A);
    WRITE_MEM(REG_SP-2, REG_F);
    REG_SP -= 2;    
    return MCYCLE_4;
}

static opcycles_t ld_hl_sp_i8(){        // 0xF8
    uint8_t value = READ_NEXT_BYTE();
    REG_H = U8((REG_SP + i8(value)) >> 8);
    REG_L = U8((REG_SP + i8(value)) & 0xFF);
    return MCYCLE_3;

}

static opcycles_t ld_sp_hl(){           // 0xF9
    REG_SP = (address_t)HL();
    return MCYCLE_2;
}

static opcycles_t ld_a_mem_nn(){        // 0xFA
    REG_A = READ_MEM(READ_NEXT_WORD());
    return MCYCLE_4;
}

static opcycles_t cp_n(){               // 0xFE
    CP_SET_FLAGS(REG_A, READ_NEXT_BYTE());
    return MCYCLE_2;
}


opcode_def_t *opcodes[512] = {
    [0x00] = &nop,
    [0x01] = &ld_bc_nn,
    [0x02] = &ld_mem_bc_a,
    [0x03] = &inc_bc,
    [0x04] = &inc_b,
    [0x05] = &dec_b,
    [0x06] = &ld_b_n,
    [0x07] = &rlca,
    [0x08] = &ld_mem_nn_sp,
    [0x09] = &add_hl_bc,
    [0x0A] = &ld_a_mem_bc,
    [0x0B] = &dec_bc,
    [0x0C] = &inc_c,
    [0x0D] = &dec_c,
    [0x0E] = &ld_c_n,
    [0x0F] = &rrca,
    [0x10] = &stop,
    [0x11] = &ld_mem_de_nn,
    [0x12] = &ld_mem_de_a,
    [0x13] = &inc_de,
    [0x14] = &inc_d,
    [0x15] = &dec_d,
    [0x16] = &ld_d_n,
    [0x17] = &rla,
    [0x18] = &jr_i8,
    [0x19] = &add_hl_de,
    [0x1A] = &ld_a_mem_de,  
    [0x1B] = &dec_de,
    [0x1C] = &inc_e,
    [0x1D] = &dec_e,
    [0x1E] = &ld_e_n,
    [0x1F] = &rra,
    [0x20] = &jr_nz_i8,
    [0x21] = &ld_hl_nn,
    [0x22] = &ld_hlp_a,
    [0x23] = NULL,
    [0x24] = NULL,
    [0x25] = NULL,
    [0x26] = NULL,
    [0x27] = &daa,
    [0x28] = NULL,
    [0x29] = NULL,
    [0x2A] = NULL,
    [0x2B] = NULL,
    [0x2C] = NULL,
    [0x2D] = NULL,
    [0x2E] = NULL,
    [0x2F] = NULL,
    [0x30] = NULL,
    [0x31] = &ld_sp_nn,
    [0x32] = NULL,
    [0x33] = NULL,
    [0x34] = NULL,
    [0x35] = NULL,
    [0x36] = &ld_mem_hl_n,
    [0x37] = NULL,
    [0x38] = NULL,
    [0x39] = NULL,
    [0x3A] = NULL,
    [0x3B] = NULL,
    [0x3C] = NULL,
    [0x3D] = NULL,
    [0x3E] = &ld_a_n,
    [0x3F] = NULL,
    [0x40] = NULL,
    [0x41] = NULL,
    [0x42] = NULL,
    [0x43] = NULL,
    [0x44] = NULL,
    [0x45] = NULL,
    [0x46] = NULL,
    [0x47] = &ld_b_a,
    [0x48] = NULL,
    [0x49] = NULL,
    [0x4A] = NULL,
    [0x4B] = NULL,
    [0x4C] = NULL,
    [0x4D] = &ld_c_l,
    [0x4E] = NULL,
    [0x4F] = NULL,
    [0x50] = NULL,
    [0x51] = NULL,
    [0x52] = NULL,
    [0x53] = NULL,
    [0x54] = &ld_d_h,
    [0x55] = NULL,
    [0x56] = NULL,
    [0x57] = NULL,
    [0x58] = NULL,
    [0x59] = NULL,
    [0x5A] = NULL,
    [0x5B] = NULL,
    [0x5C] = NULL,
    [0x5D] = &ld_e_l,
    [0x5E] = NULL,
    [0x5F] = NULL,
    [0x60] = NULL,
    [0x61] = NULL,
    [0x62] = &ld_h_d,
    [0x63] = NULL,
    [0x64] = NULL,
    [0x65] = NULL,
    [0x66] = NULL,
    [0x67] = NULL,
    [0x68] = NULL,
    [0x69] = NULL,
    [0x6A] = NULL,
    [0x6B] = &ld_l_e,
    [0x6C] = NULL,
    [0x6D] = NULL,
    [0x6E] = NULL,
    [0x6F] = NULL,
    [0x70] = NULL,
    [0x71] = NULL,
    [0x72] = NULL,
    [0x73] = NULL,
    [0x74] = NULL,
    [0x75] = NULL,
    [0x76] = NULL,
    [0x77] = NULL,
    [0x78] = &ld_a_b,
    [0x79] = &ld_a_c,
    [0x7A] = &ld_a_d,
    [0x7B] = &ld_a_e,
    [0x7C] = &ld_a_h,
    [0x7D] = &ld_a_l,
    [0x7E] = NULL,
    [0x7F] = NULL,
    [0x80] = NULL,
    [0x81] = NULL,
    [0x82] = NULL,
    [0x83] = NULL,
    [0x84] = NULL,
    [0x85] = NULL,
    [0x86] = NULL,
    [0x87] = NULL,
    [0x88] = NULL,
    [0x89] = NULL,
    [0x8A] = NULL,
    [0x8B] = NULL,
    [0x8C] = NULL,
    [0x8D] = NULL,
    [0x8E] = NULL,
    [0x8F] = NULL,
    [0x90] = NULL,
    [0x91] = NULL,
    [0x92] = NULL,
    [0x93] = NULL,
    [0x94] = NULL,
    [0x95] = NULL,
    [0x96] = NULL,
    [0x97] = NULL,
    [0x98] = NULL,
    [0x99] = NULL,
    [0x9A] = NULL,
    [0x9B] = NULL,
    [0x9C] = NULL,
    [0x9D] = NULL,
    [0x9E] = NULL,
    [0x9F] = NULL,
    [0xA0] = NULL,
    [0xA1] = NULL,
    [0xA2] = NULL,
    [0xA3] = NULL,
    [0xA4] = NULL,
    [0xA5] = NULL,
    [0xA6] = NULL,
    [0xA7] = NULL,
    [0xA8] = NULL,
    [0xA9] = NULL,
    [0xAA] = NULL,
    [0xAB] = NULL,
    [0xAC] = NULL,
    [0xAD] = NULL,
    [0xAE] = NULL,
    [0xAF] = NULL,
    [0xB0] = NULL,
    [0xB1] = NULL,
    [0xB2] = NULL,
    [0xB3] = NULL,
    [0xB4] = NULL,
    [0xB5] = NULL,
    [0xB6] = NULL,
    [0xB7] = NULL,
    [0xB8] = NULL,
    [0xB9] = NULL,
    [0xBA] = NULL,
    [0xBB] = NULL,
    [0xBC] = NULL,
    [0xBD] = NULL,
    [0xBE] = NULL,
    [0xBF] = NULL,
    [0xC0] = NULL,
    [0xC1] = NULL,
    [0xC2] = &jp_nz_nn,
    [0xC3] = &jp_nn,
    [0xC4] = NULL,
    [0xC5] = NULL,
    [0xC6] = NULL,
    [0xC7] = NULL,
    [0xC8] = NULL,
    [0xC9] = NULL,
    [0xCA] = NULL,
    [0xCB] = NULL,
    [0xCC] = NULL,
    [0xCD] = NULL,
    [0xCE] = NULL,
    [0xCF] = NULL,
    [0xD0] = NULL,
    [0xD1] = NULL,
    [0xD2] = NULL,
    [0xD3] = NULL,
    [0xD4] = NULL,
    [0xD5] = NULL,
    [0xD6] = NULL,
    [0xD7] = NULL,
    [0xD8] = NULL,
    [0xD9] = NULL,
    [0xDA] = NULL,
    [0xDB] = NULL,
    [0xDC] = NULL,
    [0xDD] = NULL,
    [0xDE] = NULL,
    [0xDF] = NULL,
    [0xE0] = NULL,
    [0xE1] = &pop_hl,
    [0xE2] = NULL,
    [0xE3] = NULL,
    [0xE4] = NULL,
    [0xE5] = &push_hl,
    [0xE6] = NULL,
    [0xE7] = NULL,
    [0xE8] = NULL,
    [0xE9] = NULL,
    [0xEA] = &ld_mm_nn_a,
    [0xEB] = NULL,
    [0xEC] = NULL,
    [0xED] = NULL,
    [0xEE] = NULL,
    [0xEF] = NULL,
    [0xF0] = NULL,
    [0xF1] = &pop_af,
    [0xF2] = NULL,
    [0xF3] = &di,
    [0xF4] = NULL,
    [0xF5] = &push_af,
    [0xF6] = NULL,
    [0xF7] = NULL,
    [0xF8] = &ld_hl_sp_i8,
    [0xF9] = &ld_sp_hl,
    [0xFA] = &ld_a_mem_nn,
    [0xFB] = NULL,
    [0xFC] = NULL,
    [0xFD] = NULL,
    [0xFE] = &cp_n,
    [0xFF] = NULL,
};