#include "opcodes.h"
#include "log.h"


static gbcycles_t nop(Gameboy *gb){                            // 0x00
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_bc_nn(Gameboy *gb){                       // 0x01
    uint16_t value = gb->read(gb, gb->pc+2) << 8 | gb->read(gb, gb->pc+1);
    gb->b = value >> 8;
    gb->c = value & 0xFF;
    gb->pc += 3;
    return MCYCLE_3;
}

static gbcycles_t ld_mem_bc_a(Gameboy *gb){     // 0x02
    uint16_t address = (uint16_t)(gb->b) << 8 | gb->c;
    gb->write(gb, address, gb->a);
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t inc_bc(Gameboy *gb){         // 0x03
    
    if (gb->c == 0xFF){
        gb->c++; // will overflow to 0x00
        gb->b++;
    } else {
        gb->c++;
    }

    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t inc_b(Gameboy *gb){          // 0x04
    gb->b++;
    gb->pc++;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    if(gb->b & 0xf == 0xf){
        gb->f |= FLAG_H;
    }

    return MCYCLE_1;
}

static gbcycles_t dec_b(Gameboy *gb){          // 0x05
    gb->b--;

    gb->f &= ~(FLAG_Z | FLAG_H);
    gb->f |= FLAG_N;

    if(gb->b == 0x00){
        gb->f |= FLAG_Z;
    }

    if(gb->b & 0xf == 0x0f){
        gb->f |= FLAG_H;
    }

    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_b_n(Gameboy *gb){         // 0x06
    gb->b = gb->read(gb, gb->pc+1);
    gb->pc += 2;
    return MCYCLE_2;
}

static gbcycles_t rlca(Gameboy *gb){          // 0x07
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if (gb->a & 0x80){
        gb->f |= FLAG_C;
        gb->a = (gb->a << 1) + 1;
    } else {
        gb->a <<= 1;
    }

    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_mem_nn_sp(Gameboy *gb){     // 0x08
    uint16_t value = gb->read(gb, gb->pc+2) << 8 | gb->read(gb, gb->pc+1);
    gb->write(gb, value, gb->sp & 0xFF);
    gb->write(gb, value+1, gb->sp >> 8);
    gb->pc += 3;
    return MCYCLE_5;
}

static gbcycles_t add_hl_bc(Gameboy *gb){        // 0x09
    uint32_t hl = (uint16_t)(gb->h) << 8 | gb->l;
    uint32_t bc = (uint16_t)(gb->b) << 8 | gb->c;
    uint32_t result = hl + bc;

    gb->f &= ~(FLAG_N | FLAG_H | FLAG_C);
    
    if(result & 0x10000){gb->f |= FLAG_C;}
    if((hl^bc^result) & 0x1000){gb->f |= FLAG_H;}
    
    gb->h = (result >> 8) & 0xFF;
    gb->l = result & 0xFF;
    gb->pc++;

    return MCYCLE_2;
}

static gbcycles_t ld_a_mem_bc(Gameboy *gb){      // 0x0A
    uint16_t address = (uint16_t)(gb->b) << 8 | gb->c;
    gb->a = gb->read(gb, address);
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t dec_bc(Gameboy *gb){          // 0x0B
    if(gb->c == 0x00){
        gb->c--;
        gb->b--;
    }else{
        gb->c--;
    }
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t inc_c(Gameboy *gb){          // 0x0C
    gb->c++;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    if(gb->c & 0xf == 0xf){
        gb->f |= FLAG_H;
    }

    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t dec_c(Gameboy *gb){          // 0x0D
    gb->c--;

    gb->f &= ~(FLAG_Z | FLAG_H);
    gb->f |= FLAG_N;

    if(gb->c == 0x00){
        gb->f |= FLAG_Z;
    }

    if(gb->c & 0xf == 0x0f){
        gb->f |= FLAG_H;
    }

    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_c_n(Gameboy *gb){          // 0x0E
    gb->c = gb->read(gb, gb->pc+1);
    gb->pc += 2;
    return MCYCLE_2;
}

static gbcycles_t rrca(Gameboy *gb){          // 0x0F
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if (gb->a & 0x01){
        gb->f |= FLAG_C;
        gb->a = (gb->a >> 1) + 0x80;
    } else {
        gb->a >>= 1;
    }

    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t stop(Gameboy *gb){             // 0x10
    
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_de_nn(Gameboy *gb){       // 0x11
    uint16_t value = gb->read(gb, gb->pc+2) << 8 | gb->read(gb, gb->pc+1);
    gb->d = value >> 8;
    gb->e = value & 0xFF;
    gb->pc += 3;
    return MCYCLE_3;
}

static gbcycles_t ld_mem_de_a(Gameboy *gb){        // 0x12 
    uint16_t address = (uint16_t)(gb->d) << 8 | gb->e;
    gb->write(gb, address, gb->a);
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t inc_de(Gameboy *gb){             // 0x13
    if(gb->e == 0xFF){
        gb->e++;
        gb->d++;
    }else{
        gb->e++;
    }
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t inc_d(Gameboy *gb){              // 0x14
    gb->d++;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    if(gb->d & 0xf == 0xf){
        gb->f |= FLAG_H;
    }

    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t dec_d(Gameboy *gb){              // 0x15
    gb->d--;

    gb->f &= ~(FLAG_Z | FLAG_H);
    gb->f |= FLAG_N;

    if(gb->d == 0x00){
        gb->f |= FLAG_Z;
    }

    if(gb->d & 0xf == 0x0f){
        gb->f |= FLAG_H;
    }

    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_d_n(Gameboy *gb){             // 0x16
    gb->d = gb->read(gb, gb->pc+1);
    gb->pc += 2;
    return MCYCLE_2;
}

static gbcycles_t rla(Gameboy *gb){              // 0x17

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    uint8_t prev_carry = gb->f & FLAG_C;

    if (gb->a & 0x80){
        gb->f |= FLAG_C;
    }else{
        gb->f &= ~FLAG_C;
    }

    gb->a = (gb->a << 1) & 0xFF;

    if (prev_carry){
        gb->a |= 0x01;
    }

    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t jr_i8(Gameboy *gb){             // 0x18
    int8_t value = (int8_t)gb->read(gb, gb->pc+1);
    gb->pc += value + 2;
    return MCYCLE_3;
}

static gbcycles_t add_hl_de(Gameboy *gb){         // 0x19
    uint32_t hl = (uint16_t)(gb->h) << 8 | gb->l;
    uint32_t de = (uint16_t)(gb->d) << 8 | gb->e;
    uint32_t result = hl + de;

    gb->f &= ~(FLAG_N | FLAG_H | FLAG_C);
    
    if(result & 0x10000){gb->f |= FLAG_C;}
    if((hl^de^result) & 0x1000){gb->f |= FLAG_H;}

    gb->h = (result >> 8) & 0xFF;
    gb->l = result & 0xFF;
    gb->pc++;


    return MCYCLE_2;
}

static gbcycles_t ld_a_mem_de(Gameboy *gb){       // 0x1A
    uint16_t address = (uint16_t)(gb->d) << 8 | gb->e;
    gb->a = gb->read(gb, address);
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t dec_de(Gameboy *gb){             // 0x1B
    if(gb->e == 0x00){
        gb->e--;
        gb->d--;
    }else{
        gb->e--;
    }
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t inc_e(Gameboy *gb){              // 0x1C
    gb->e++;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    if(gb->e & 0xf == 0xf){
        gb->f |= FLAG_H;
    }

    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t dec_e(Gameboy *gb){              // 0x1D
    gb->e--;

    gb->f &= ~(FLAG_Z | FLAG_H);
    gb->f |= FLAG_N;

    if(gb->e == 0x00){
        gb->f |= FLAG_Z;
    }

    if(gb->e & 0xf == 0x0f){
        gb->f |= FLAG_H;
    }

    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_e_n(Gameboy *gb){             // 0x1E
    gb->e = gb->read(gb, gb->pc+1);
    gb->pc += 2;
    return MCYCLE_2;
}

static gbcycles_t rra(Gameboy *gb){              // 0x1F
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    uint8_t prev_carry = gb->f & FLAG_C;

    if (gb->a & 0x01){
        gb->f |= FLAG_C;
    }else{
        gb->f &= ~FLAG_C;
    }

    gb->a = (gb->a >> 1) & 0xFF;
    
    if (prev_carry){
        gb->a |= 0x80;
    }

    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t jr_nz_i8(Gameboy *gb){         // 0x20
    if(!(gb->f & FLAG_Z)){
        int8_t value = (int8_t)gb->read(gb, gb->pc+1);
        gb->pc += value + 2;
        return MCYCLE_3;
    }
    gb->pc += 2;
    return MCYCLE_2;
}

static gbcycles_t ld_hl_nn(Gameboy *gb){           // 0x21
    uint16_t value = gb->read(gb, gb->pc+2) << 8 | gb->read(gb, gb->pc+1);
    gb->h = value >> 8;
    gb->l = value & 0xFF;
    gb->pc += 3;
    return MCYCLE_3;
}

static gbcycles_t ld_hlp_a(Gameboy *gb){            // 0x22 
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;
    gb->write(gb, address, gb->a);
    
    if(gb->l == 0xFF){
        gb->l++;
        gb->h++;
    }else{
        gb->l++;
    }
    
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t inc_hl(Gameboy *gb){            // 0x23
    if(gb->l == 0xFF){
        gb->l++;
        gb->h++;
    }else{
        gb->l++;
    }
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t inc_h(Gameboy *gb){             // 0x24
    gb->h++;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    if(gb->h & 0xf == 0xf){
        gb->f |= FLAG_H;
    }

    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t dec_h(Gameboy *gb){             // 0x25
    gb->h--;

    gb->f &= ~(FLAG_Z | FLAG_H);
    gb->f |= FLAG_N;

    if(gb->h == 0x00){
        gb->f |= FLAG_Z;
    }

    if(gb->h & 0xf == 0x0f){
        gb->f |= FLAG_H;
    }

    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_h_n(Gameboy *gb){             // 0x26
    gb->h = gb->read(gb, gb->pc+1);
    gb->pc += 2;
    return MCYCLE_2;
}

static gbcycles_t daa(Gameboy *gb){              // 0x27

    uint16_t result = (uint16_t)(gb->a); 

    gb->f &= ~(FLAG_Z);

    if(gb->f & FLAG_N){
        if(gb->f & FLAG_H){
            result = (result - 0x06) & 0xFF;
        }

        if(gb->f & FLAG_C){
            result -= 0x60;
        }
    }else{
        if(gb->f & FLAG_H || (result & 0x0F) > 0x09){
            result += 0x06;
        }

        if(gb->f & FLAG_C || result > 0x9F){
            result += 0x60;
        }
    }

    if(result == 0){
        gb->f |= FLAG_Z;
    }

    if((result & 0x100) == 0x100){
        gb->f |= FLAG_C;
    }

    gb->f &= ~(FLAG_H);

    gb->a = (uint8_t)result;

    gb->pc++;

    return MCYCLE_1;
}

static gbcycles_t jr_z_i8(Gameboy *gb){             // 0x28
    if (gb->f & FLAG_Z){
        int8_t value = (int8_t)gb->read(gb, gb->pc+1);
        gb->pc += value + 2;
        return MCYCLE_3;
    }
    gb->pc += 2;
    return MCYCLE_2;
}

static gbcycles_t add_hl_hl(Gameboy *gb){         // 0x29
    uint32_t hl = (uint16_t)(gb->h) << 8 | gb->l;
    uint32_t result = hl + hl;

    gb->f &= ~(FLAG_N | FLAG_H | FLAG_C);
    
    if(result & 0x10000){gb->f |= FLAG_C;}
    if((hl^hl^result) & 0x1000){gb->f |= FLAG_H;}

    gb->h = (result >> 8) & 0xFF;
    gb->l = result & 0xFF;
    gb->pc++;

    return MCYCLE_2;
}

static gbcycles_t ld_a_mem_hlp(Gameboy *gb){       // 0x2A
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;
    gb->a = gb->read(gb, address);

    if(gb->l == 0xFF){
        gb->l++;
        gb->h++;
    }else{
        gb->l++;
    }
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t dec_hl(Gameboy *gb){            // 0x2B
    if(gb->l == 0x00){
        gb->l--;
        gb->h--;
    }else{ 
        gb->l--;
    }
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t inc_l(Gameboy *gb){            // 0x2C
    gb->l++;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    if(gb->l & 0xf == 0xf){
        gb->f |= FLAG_H;
    }

    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t dec_l(Gameboy *gb){            // 0x2D
    gb->l--;

    gb->f &= ~(FLAG_Z | FLAG_H);
    gb->f |= FLAG_N;

    if(gb->l == 0x00){
        gb->f |= FLAG_Z;
    }

    if(gb->l & 0xf == 0x0f){
        gb->f |= FLAG_H;
    }

    gb->pc++;
    return MCYCLE_1;
}





static gbcycles_t ld_sp_nn(Gameboy *gb){           // 0x31
    uint16_t value = gb->read(gb, gb->pc+2) << 8 | gb->read(gb, gb->pc+1);
    gb->sp = value;
    gb->pc += 3;
    return MCYCLE_3;
}

static gbcycles_t ld_mem_hl_n(Gameboy *gb){       // 0x36
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;
    gb->write(gb, address, gb->read(gb, gb->pc+1));
    gb->pc += 2;
    return MCYCLE_3;
}

static gbcycles_t inc_a(Gameboy *gb){             // 0x3C
    gb->a++;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    if(gb->a & 0xf == 0xf){
        gb->f |= FLAG_H;
    }

    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_a_n(Gameboy *gb){             // 0x3E
    gb->a = gb->read(gb, gb->pc+1);
    gb->pc += 2;
    return MCYCLE_2;
}

static gbcycles_t ld_b_a(Gameboy *gb){             // 0x47
    gb->b = gb->a;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_c_l(Gameboy *gb){             // 0x4D
    gb->c = gb->l;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_d_h(Gameboy *gb){             //0x54 
    gb->d = gb->h;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_e_l(Gameboy *gb){             // 0x5D
    gb->e = gb->l;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_h_d(Gameboy *gb){             // 0x62
    gb->h = gb->d;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_l_e(Gameboy *gb){             // 0x6B 
    gb->l = gb->e;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_mem_hl_a(Gameboy *gb){       // 0x77
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;
    gb->write(gb, address, gb->a);
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t ld_a_b(Gameboy *gb){             // 0x78
    gb->a = gb->b;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_a_c(Gameboy *gb){             // 0x79
    gb->a = gb->c;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_a_d(Gameboy *gb){             // 0x7A
    gb->a = gb->d;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_a_e(Gameboy *gb){             // 0x7B
    gb->a = gb->e;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_a_h(Gameboy *gb){             // 0x7C
    gb->a = gb->h;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_a_l(Gameboy *gb){             // 0x7D
    gb->a = gb->l;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t and_e(Gameboy *gb){            // 0xA3
    uint8_t result = gb->a & gb->e;
    gb->f &= ~(FLAG_Z | FLAG_N |  FLAG_C);
    gb->f |= FLAG_H;

    if(!result){gb->f |= FLAG_Z;}

    gb->a = result;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t xor_a(Gameboy *gb){             // 0xA9
    uint8_t result = gb->a ^ gb->a;
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if(!result){gb->f |= FLAG_Z;}

    gb->a = result;
    gb->pc++;
}

static gbcycles_t or_c(Gameboy *gb){              // 0xB1
    // OR_SET_FLAGS(REG_A, REG_C);
    uint8_t result = gb->a | gb->c;
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if(!result){gb->f |= FLAG_Z;}

    gb->a = result;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t pop_bc(Gameboy *gb){            // 0xC1

    gb->c = gb->read(gb, gb->sp);
    gb->b = gb->read(gb, gb->sp+1);
    gb->sp += 2;
    gb->pc++;
    return MCYCLE_3;
}

static gbcycles_t jp_nz_nn(Gameboy *gb){          // 0xC2
    uint16_t address = gb->read(gb, gb->pc+2) << 8 | gb->read(gb, gb->pc+1);
    if(!(gb->f & FLAG_Z)){
        gb->pc = address;
        return MCYCLE_4;
    }
    gb->pc += 3;
    return MCYCLE_3;
}

static gbcycles_t jp_nn(Gameboy *gb){              // 0xC3
    uint16_t address = gb->read(gb, gb->pc+2) << 8 | gb->read(gb, gb->pc+1);
    gb->pc = address;
    return MCYCLE_4;
}

static gbcycles_t call_nz_nn(Gameboy *gb){        // 0xC4

    uint16_t address = gb->read(gb, gb->pc+2) << 8 | gb->read(gb, gb->pc+1);
    if(!(gb->f & FLAG_Z)){
        gb->write(gb, gb->sp-1, gb->pc >> 8);
        gb->write(gb, gb->sp-2, gb->pc & 0xFF);
        gb->sp -= 2;
        gb->pc = address;
        return MCYCLE_6;
    }
    gb->pc += 3;
    return MCYCLE_3;
}

static gbcycles_t push_bc(Gameboy *gb){            // 0xC5
    gb->write(gb, gb->sp-1, gb->b);
    gb->write(gb, gb->sp-2, gb->c);
    gb->sp -= 2;
    gb->pc++;
    return MCYCLE_4;
}

static gbcycles_t ret(Gameboy *gb){                // 0xC9
    gb->pc = (gb->read(gb, gb->sp+1) << 8 | gb->read(gb, gb->sp))+2;
    gb->sp += 2;
    gb->pc++;
    return MCYCLE_4;
}

static gbcycles_t call_nn(Gameboy *gb){            // 0xCD
    uint16_t address = gb->read(gb, gb->pc+2) << 8 | gb->read(gb, gb->pc+1);
    gb->write(gb, gb->sp-1, gb->pc >> 8);
    gb->write(gb, gb->sp-2, gb->pc & 0xFF);
    gb->sp -= 2;
    gb->pc = address;
    return MCYCLE_6;
}

static gbcycles_t ldh_mem_n_a(Gameboy *gb){        // 0xE0
    uint8_t value = gb->read(gb, gb->pc+1);
    gb->write(gb, _IO + value, gb->a);
    gb->pc += 2;
    return MCYCLE_3;
}

static gbcycles_t pop_hl(Gameboy *gb){             // 0xE1
    gb->l = gb->read(gb, gb->sp);
    gb->h = gb->read(gb, gb->sp+1);
    gb->sp += 2;
    gb->pc++;
    return MCYCLE_3;
}

static gbcycles_t push_hl(Gameboy *gb){             // 0xE5
    gb->write(gb, gb->sp-1, gb->h);
    gb->write(gb, gb->sp-2, gb->l);
    gb->sp -= 2;
    gb->pc++;
    return MCYCLE_4;
}

static gbcycles_t and_n(Gameboy *gb){             // 0xE6
    uint8_t value = gb->read(gb, gb->pc+1);
    uint8_t result = gb->a & value;
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);
    gb->f |= FLAG_H;

    if(!result){gb->f |= FLAG_Z;}

    gb->a = result;
    gb->pc += 2;
    return MCYCLE_2;
}


static gbcycles_t ld_mm_nn_a(Gameboy *gb){         // 0xEA
    uint16_t address = gb->read(gb, gb->pc+2) << 8 | gb->read(gb, gb->pc+1);
    gb->write(gb, address, gb->a);
    gb->pc += 3;
    return MCYCLE_4;
}


static gbcycles_t ldh_a_n(Gameboy *gb){             // 0xF0
    // REG_A = READ_MEM(0xFF00 + READ_NEXT_BYTE());
    uint8_t value = gb->read(gb, gb->pc+1);
    gb->a = gb->read(gb, _IO + value);
    gb->pc += 2;
    return MCYCLE_3;
}


static gbcycles_t pop_af(Gameboy *gb){             // 0xF1
    gb->a = gb->read(gb, gb->sp+1);
    gb->f = gb->read(gb, gb->sp);
    gb->sp += 2;
    gb->pc++;
    return MCYCLE_3;
}

static gbcycles_t di(Gameboy *gb){                 // 0xF3
    gb->ie = 0x00;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t push_af(Gameboy *gb){             // 0xF5
    gb->write(gb, gb->sp-1, gb->a);
    gb->write(gb, gb->sp-2, gb->f);
    gb->sp -= 2;
    gb->pc++;
    return MCYCLE_4;
}

static gbcycles_t ld_hl_sp_i8(Gameboy *gb){        // 0xF8
    int8_t value = (int8_t)gb->read(gb, gb->pc+1);
    uint16_t sp = gb->sp + value;

    gb->h = (sp >> 8) & 0xFF;
    gb->l = sp & 0xFF;

    gb->pc += 2;

    return MCYCLE_3;
}

static gbcycles_t ld_sp_hl(Gameboy *gb){           // 0xF9
    uint16_t value = (uint16_t)(gb->h) << 8 | gb->l;
    gb->sp = value;
    gb->pc++;
    return MCYCLE_2;
}


static gbcycles_t ld_a_mem_nn(Gameboy *gb){        // 0xFA
    // REG_A = READ_MEM(READ_NEXT_WORD());
    uint16_t value = gb->read(gb, gb->pc+2) << 8 | gb->read(gb, gb->pc+1);
    gb->a = gb->read(gb, value);
    gb->pc += 3;
    return MCYCLE_4;
}

static gbcycles_t cp_n(Gameboy *gb){               // 0xFE
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->read(gb, gb->pc+1);

    uint16_t result = a - b;

    gb->f &= ~(FLAG_Z |  FLAG_H | FLAG_C);
    gb->f |= FLAG_H;

    if((a^b^result) & 0x10){gb->f |= FLAG_H;}

    if (result & 0x100 != 0){gb->f |= FLAG_C;}    

    gb->pc += 2;
    return MCYCLE_2;
}

static gbcycles_t rst_38(Gameboy *gb){             // 0xFF
    // log_info("pch: %02X, pcl: %02X", (REG_PC+1) >> 8, (REG_PC+1) & 0xFF);
    // WRITE_MEM(REG_SP-1, (REG_PC+1) >> 8);
    // WRITE_MEM(REG_SP-2, (REG_PC+1) & 0xFF);
    // REG_SP -= 2;
    // REG_PC = 0x0038-1;
    return MCYCLE_4;
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
    [0x11] = &ld_de_nn,
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
    [0x23] = &inc_hl,
    [0x24] = &inc_h,
    [0x25] = &dec_h,
    [0x26] = &ld_h_n,
    [0x27] = &daa,
    [0x28] = &jr_z_i8,
    [0x29] = &add_hl_hl,
    [0x2A] = &ld_a_mem_hlp,
    [0x2B] = &dec_hl,
    [0x2C] = &inc_l,
    [0x2D] = &dec_l,
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
    [0x3C] = &inc_a,
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
    [0x77] = &ld_mem_hl_a,
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
    [0xA3] = &and_e,
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
    [0xB1] = &or_c,
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
    [0xC1] = &pop_bc,
    [0xC2] = &jp_nz_nn,
    [0xC3] = &jp_nn,
    [0xC4] = &call_nz_nn,
    [0xC5] = &push_bc,
    [0xC6] = NULL,
    [0xC7] = NULL,
    [0xC8] = NULL,
    [0xC9] = &ret,
    [0xCA] = NULL,
    [0xCB] = NULL,
    [0xCC] = NULL,
    [0xCD] = &call_nn,
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
    [0xE0] = &ldh_mem_n_a,
    [0xE1] = &pop_hl,
    [0xE2] = NULL,
    [0xE3] = NULL,
    [0xE4] = NULL,
    [0xE5] = &push_hl,
    [0xE6] = &and_n,
    [0xE7] = NULL,
    [0xE8] = NULL,
    [0xE9] = NULL,
    [0xEA] = &ld_mm_nn_a,
    [0xEB] = NULL,
    [0xEC] = NULL,
    [0xED] = NULL,
    [0xEE] = NULL,
    [0xEF] = NULL,
    [0xF0] = &ldh_a_n,
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
    [0xFF] = &rst_38,
};
