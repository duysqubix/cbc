#include "opcodes.h"
#include "log.h"


static gbcycles_t nop(Gameboy *gb){                            // 0x00
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_bc_nn(Gameboy *gb){                       // 0x01
    // uint16_t value = gb->read(gb, gb->pc+2) << 8 | gb->read(gb, gb->pc+1);
    // printf("u16: %04X\n", value);
    // printf("b: %02X\n", value >> 8);
    // printf("c: %02X\n", value & 0xFF);
    // gb->b = value >> 8;
    // gb->c = value & 0xFF;

    gb->b = gb->read(gb, gb->pc+2);
    gb->c = gb->read(gb, gb->pc+1);
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
    uint8_t result = gb->b + 1;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    if(result == 0x00){
        gb->f |= FLAG_Z;
    }

    if((gb->b & 0xf) == 0xf){
        gb->f |= FLAG_H;
    }

    gb->b = result;
    gb->pc++;

    return MCYCLE_1;
}

static gbcycles_t dec_b(Gameboy *gb){          // 0x05
    uint8_t result = gb->b - 1;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    if(result == 0x00){
        gb->f |= FLAG_Z;
    }

    gb->f |= FLAG_N;

    if((gb->b & 0xf) == 0x00){
        gb->f |= FLAG_H;
    }

    gb->b = result;

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
    uint8_t result = gb->c + 1;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    if(result == 0x00){
        gb->f |= FLAG_Z;
    }

    if((gb->c & 0xf) == 0xf){
        gb->f |= FLAG_H;
    }

    gb->c = result;
    gb->pc++;

    return MCYCLE_1;
}

static gbcycles_t dec_c(Gameboy *gb){          // 0x0D
    uint8_t result = gb->c - 1;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    if(result == 0x00){
        gb->f |= FLAG_Z;
    }

    gb->f |= FLAG_N;

    if((gb->c & 0xf) == 0x00){
        gb->f |= FLAG_H;
    }

    gb->c = result;
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
    uint8_t result = gb->d + 1;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    if(result == 0x00){
        gb->f |= FLAG_Z;
    }

    if((gb->d & 0xf) == 0xf){
        gb->f |= FLAG_H;
    }

    gb->d = result;
    gb->pc++;

    return MCYCLE_1;
}

static gbcycles_t dec_d(Gameboy *gb){              // 0x15
    uint8_t result = gb->d - 1;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    if(result == 0x00){
        gb->f |= FLAG_Z;
    }

    gb->f |= FLAG_N;

    if((gb->d & 0xf) == 0x00){
        gb->f |= FLAG_H;
    }

    gb->d = result;
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
    uint8_t result = gb->e + 1;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    if(result == 0x00){
        gb->f |= FLAG_Z;
    }

    if((gb->e & 0xf) == 0xf){
        gb->f |= FLAG_H;
    }

    gb->e = result;
    gb->pc++;

    return MCYCLE_1;
}

static gbcycles_t dec_e(Gameboy *gb){              // 0x1D
    uint8_t result = gb->e - 1;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    if(result == 0x00){
        gb->f |= FLAG_Z;
    }

    gb->f |= FLAG_N;

    if((gb->e & 0xf) == 0x00){
        gb->f |= FLAG_H;
    }

    gb->e = result;
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
    uint8_t result = gb->h + 1;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    if(result == 0x00){
        gb->f |= FLAG_Z;
    }

    if((gb->h & 0xf) == 0xf){
        gb->f |= FLAG_H;
    }

    gb->h = result;
    gb->pc++;

    return MCYCLE_1;
}

static gbcycles_t dec_h(Gameboy *gb){             // 0x25
    uint8_t result = gb->h - 1;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    if(result == 0x00){
        gb->f |= FLAG_Z;
    }

    gb->f |= FLAG_N;

    if((gb->h & 0xf) == 0x00){
        gb->f |= FLAG_H;
    }

    gb->h = result;
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
    uint8_t result = gb->l + 1;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    if(result == 0x00){
        gb->f |= FLAG_Z;
    }

    if((gb->l & 0xf) == 0xf){
        gb->f |= FLAG_H;
    }

    gb->l = result;
    gb->pc++;

    return MCYCLE_1;
}

static gbcycles_t dec_l(Gameboy *gb){            // 0x2D
    uint8_t result = gb->l - 1;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    if(result == 0x00){
        gb->f |= FLAG_Z;
    }

    gb->f |= FLAG_N;

    if((gb->l & 0xf) == 0x00){
        gb->f |= FLAG_H;
    }

    gb->l = result;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_l_n(Gameboy *gb){             // 0x2E
    gb->l = gb->read(gb, gb->pc+1);
    gb->pc += 2;
    return MCYCLE_2;
}

static gbcycles_t cpl(Gameboy *gb){                     // 0x2f
    gb->a = ~gb->a;
    gb->f |= (FLAG_N | FLAG_H);
    gb->pc++;
    return MCYCLE_1;
}


static gbcycles_t jr_nc_i8(Gameboy *gb){            // 0x30
    if(!(gb->f & FLAG_C)){
        int8_t value = (int8_t)gb->read(gb, gb->pc+1);
        gb->pc += value + 2;
        return MCYCLE_3;
    }
    gb->pc += 2;
    return MCYCLE_2;
}

static gbcycles_t ld_sp_nn(Gameboy *gb){           // 0x31
    uint16_t value = gb->read(gb, gb->pc+2) << 8 | gb->read(gb, gb->pc+1);
    gb->sp = value;
    gb->pc += 3;
    return MCYCLE_3;
}

static gbcycles_t ld_hlm_a(Gameboy *gb){          // 0x32
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;

    gb->write(gb, address, gb->a);

    if(gb->l == 0x00){
        gb->l--;
        gb->h--;
    }else{ 
        gb->l--;
    }

    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t inc_sp(Gameboy *gb){            // 0x33
    gb->sp++;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t inc_mem_hl(Gameboy *gb){        // 0x34
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;
    uint8_t value = gb->read(gb, address);
    uint8_t result = value + 1;
    
    gb->f &= ~(FLAG_Z | FLAG_H | FLAG_N);

    if(result == 0x00){
        gb->f |= FLAG_Z;
    }

    if((value & 0xf) == 0xf){
        gb->f |= FLAG_H;
    }

    gb->write(gb, address, result);

    gb->pc++;
    return MCYCLE_4;
    
}

static gbcycles_t dec_mem_hl(Gameboy *gb){        // 0x35
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;
    uint8_t value = gb->read(gb, address);
    uint8_t result = value - 1;

    gb->f &= ~(FLAG_Z  | FLAG_H);
    gb->f |= FLAG_N;

    if(result == 0x00){
        gb->f |= FLAG_Z;
    }

    if((value & 0xf) == 0x00){
        gb->f |= FLAG_H;
    }

    gb->write(gb, address, result);
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t ld_mem_hl_n(Gameboy *gb){       // 0x36
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;
    gb->write(gb, address, gb->read(gb, gb->pc+1));
    gb->pc += 2;
    return MCYCLE_3;
}

static gbcycles_t scf(Gameboy *gb){               //0x37
    gb->f &= ~(FLAG_N | FLAG_H);
    gb->f |= FLAG_C;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t jr_c_i8(Gameboy *gb){            // 0x38
    if(gb->f & FLAG_C){
        int8_t value = (int8_t)gb->read(gb, gb->pc+1);
        gb->pc += value + 2;
        return MCYCLE_3;
    }
    gb->pc += 2;
    return MCYCLE_2;
} 

static gbcycles_t add_hl_sp(Gameboy *gb){         // 0x39
    uint32_t hl = (uint16_t)(gb->h) << 8 | gb->l;
    uint32_t sp = gb->sp;
    uint32_t result = hl + sp;
    
    gb->f &= ~(FLAG_N | FLAG_H | FLAG_C);

    if(result == 0x0000){
        gb->f |= FLAG_Z;
    }

    if(result & 0x10000){
        gb->f |= FLAG_C;
    }

    if((hl^sp^result) & 0x1000){
        gb->f |= FLAG_H;
    }

    gb->h = (result >> 8) & 0xFF;
    gb->l = result & 0xFF;
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t ld_a_mem_hl_dec(Gameboy *gb){       // 0x3A
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;
    gb->a = gb->read(gb, address);

    if(gb->l == 0x00){
        gb->l--;
        gb->h--;
    }else{ 
        gb->l--;
    }

    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t dec_sp(Gameboy *gb){            // 0x3B
    gb->sp--;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t inc_a(Gameboy *gb){             // 0x3C
    uint8_t result = gb->a + 1;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H);

    if(result == 0x00){
        gb->f |= FLAG_Z;
    }

    if((gb->a & 0xf) == 0xf){
        gb->f |= FLAG_H;
    }

    gb->a = result;
    gb->pc++;

    return MCYCLE_1;
}

static gbcycles_t dec_a(Gameboy *gb){             // 0x3D
    uint8_t result = gb->a - 1;

    gb->f &= ~(FLAG_Z | FLAG_H);
    gb->f |= FLAG_N;


    if(result == 0x00){
        gb->f |= FLAG_Z;
    }

    if((gb->a & 0xf) == 0x00){
        gb->f |= FLAG_H;
    }

    gb->a = result;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_a_n(Gameboy *gb){             // 0x3E
    gb->a = gb->read(gb, gb->pc+1);
    gb->pc += 2;
    return MCYCLE_2;
}

static gbcycles_t ccf(Gameboy *gb){              // 0x3F
    gb->f &= ~(FLAG_N | FLAG_H);
    gb->f ^= FLAG_C;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_b_b(Gameboy *gb){            // 0x40
    gb->b = gb->b;
    gb->pc++;
    return MCYCLE_1;
}
static gbcycles_t ld_b_c(Gameboy *gb){            // 0x41
    gb->b = gb->c;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_b_d(Gameboy *gb){            // 0x42
    gb->b = gb->d;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_b_e(Gameboy *gb){            // 0x43
    gb->b = gb->e;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_b_h(Gameboy *gb){            // 0x44
    gb->b = gb->h;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_b_l(Gameboy *gb){            // 0x45
    gb->b = gb->l;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_b_mem_hl(Gameboy *gb){       // 0x46
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;
    gb->b = gb->read(gb, address);
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t ld_b_a(Gameboy *gb){             // 0x47
    gb->b = gb->a;
    gb->pc++;
    return MCYCLE_1;
}
static gbcycles_t ld_c_b(Gameboy *gb){            // 0x48
    gb->c = gb->b;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_c_c(Gameboy *gb){             // 0x49
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_c_d(Gameboy *gb){             // 0x4A
    gb->c = gb->d;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_c_e(Gameboy *gb){             // 0x4B
    gb->c = gb->e;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_c_h(Gameboy *gb){             // 0x4C
    gb->c = gb->h;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_c_l(Gameboy *gb){             // 0x4D
    gb->c = gb->l;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_c_mem_hl(Gameboy *gb){       // 0x4E
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;
    gb->c = gb->read(gb, address);
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t ld_c_a(Gameboy *gb){             // 0x4F
    gb->c = gb->a;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_d_b(Gameboy *gb){             // 0x50
    gb->d = gb->b;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_d_c(Gameboy *gb){             // 0x51
    gb->d = gb->c;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_d_d(Gameboy *gb){             // 0x52
    gb->d = gb->d;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_d_e(Gameboy *gb){             // 0x53
    gb->d = gb->e;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_d_h(Gameboy *gb){             //0x54 
    gb->d = gb->h;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_d_l(Gameboy *gb){             // 0x55
    gb->d = gb->l;
    gb->pc++;
    return MCYCLE_1;
}


static gbcycles_t ld_d_mem_hl(Gameboy *gb){       // 0x56
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;
    gb->d = gb->read(gb, address);
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t ld_d_a(Gameboy *gb){             // 0x57
    gb->d = gb->a;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_e_b(Gameboy *gb){             // 0x58
    gb->e = gb->b;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_e_c(Gameboy *gb){             // 0x59
    gb->e = gb->c;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_e_d(Gameboy *gb){             // 0x5A
    gb->e = gb->d;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_e_e(Gameboy *gb){             // 0x5B
    gb->e = gb->e;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_e_h(Gameboy *gb){             // 0x5C
    gb->e = gb->h;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_e_l(Gameboy *gb){             // 0x5D
    gb->e = gb->l;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_e_mem_hl(Gameboy *gb){       // 0x5E
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;
    gb->e = gb->read(gb, address);
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t ld_e_a(Gameboy *gb){             // 0x5F
    gb->e = gb->a;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_h_b(Gameboy *gb){             // 0x60
    gb->h = gb->b;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_h_c(Gameboy *gb){             // 0x61
    gb->h = gb->c;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_h_d(Gameboy *gb){             // 0x62
    gb->h = gb->d;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_h_e(Gameboy *gb){             // 0x63
    gb->h = gb->e;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_h_h(Gameboy *gb){             // 0x64
    gb->h = gb->h;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_h_l(Gameboy *gb){             // 0x65
    gb->h = gb->l;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_h_mem_hl(Gameboy *gb){       // 0x66
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;
    gb->h = gb->read(gb, address);
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t ld_h_a(Gameboy *gb){             // 0x67
    gb->h = gb->a;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_l_b(Gameboy *gb){             // 0x68
    gb->l = gb->b;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_l_c(Gameboy *gb){             // 0x69
    gb->l = gb->c;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_l_d(Gameboy *gb){             // 0x6A
    gb->l = gb->d;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_l_e(Gameboy *gb){             // 0x6B
    gb->l = gb->e;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_l_h(Gameboy *gb){             // 0x6C
    gb->l = gb->h;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_l_l(Gameboy *gb){             // 0x6D
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_l_mem_hl(Gameboy *gb){       // 0x6E
    uint16_t hl = (uint16_t)gb->h << 8 | (uint16_t)gb->l;
    gb->l = gb->read(gb, hl);
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t ld_l_a(Gameboy *gb){             // 0x6F
    gb->l = gb->a;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ld_mem_hl_b(Gameboy *gb){       // 0x70
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;
    gb->write(gb, address, gb->b);
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t ld_mem_hl_c(Gameboy *gb){       // 0x71
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;
    gb->write(gb, address, gb->c);
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t ld_mem_hl_d(Gameboy *gb){       // 0x72
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;
    gb->write(gb, address, gb->d);
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t ld_mem_hl_e(Gameboy *gb){       // 0x73
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;
    gb->write(gb, address, gb->e);
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t ld_mem_hl_h(Gameboy *gb){       // 0x74
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;
    gb->write(gb, address, gb->h);
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t ld_mem_hl_l(Gameboy *gb){       // 0x75
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;
    gb->write(gb, address, gb->l);
    gb->pc++;
    return MCYCLE_2;
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

static gbcycles_t ld_a_mem_hl(Gameboy *gb){       // 0x7E
    uint16_t address = (uint16_t)(gb->h) << 8 | gb->l;
    gb->a = gb->read(gb, address);
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t ld_a_a(Gameboy *gb){             // 0x7F
    gb->a = gb->a;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t add_a_b(Gameboy *gb){            // 0x80
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->b;
    uint16_t result = a + b;
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if ((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }

    if(((a^b^result) & 0x10)){
        gb->f |= FLAG_H;
    }

    if((result & 0x100) != 0){
        gb->f |= FLAG_C;
    }

    gb->a = result & 0xff;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t add_a_c(Gameboy *gb){            // 0x81
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->c;
    uint16_t result = a + b;
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if ((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }

    if(((a^b^result) & 0x10)){
        gb->f |= FLAG_H;
    }

    if((result & 0x100) != 0){
        gb->f |= FLAG_C;
    }

    gb->a = result & 0xff;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t add_a_d(Gameboy *gb){            // 0x82
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->d;
    uint16_t result = a + b;
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if ((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }

    if(((a^b^result) & 0x10)){
        gb->f |= FLAG_H;
    }

    if((result & 0x100) != 0){
        gb->f |= FLAG_C;
    }

    gb->a = result & 0xff;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t add_a_e(Gameboy *gb){            // 0x83
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->e;
    uint16_t result = a + b;
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if ((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }

    if(((a^b^result) & 0x10)){
        gb->f |= FLAG_H;
    }

    if((result & 0x100) != 0){
        gb->f |= FLAG_C;
    }

    gb->a = result & 0xff;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t add_a_h(Gameboy *gb){            // 0x84
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->h;
    uint16_t result = a + b;
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if ((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }

    if(((a^b^result) & 0x10)){
        gb->f |= FLAG_H;
    }

    if((result & 0x100) != 0){
        gb->f |= FLAG_C;
    }

    gb->a = result & 0xff;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t add_a_l(Gameboy *gb){            // 0x85
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->l;
    uint16_t result = a + b;
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if ((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }

    if(((a^b^result) & 0x10)){
        gb->f |= FLAG_H;
    }

    if((result & 0x100) != 0){
        gb->f |= FLAG_C;
    }

    gb->a = result & 0xff;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t add_a_mem_hl(Gameboy *gb){      // 0x86
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->read(gb, (uint16_t)(gb->h << 8 | gb->l));
    uint16_t result = a + b;
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if ((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }

    if(((a^b^result) & 0x10)){
        gb->f |= FLAG_H;
    }

    if((result & 0x100) != 0){
        gb->f |= FLAG_C;
    }

    gb->a = result & 0xff;
    gb->pc++;
    return MCYCLE_2;
}

static gbcycles_t add_a_a(Gameboy *gb){            // 0x87
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->a;
    uint16_t result = a + b;
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if ((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }

    if(((a^b^result) & 0x10)){
        gb->f |= FLAG_H;
    }

    if((result & 0x100) != 0){
        gb->f |= FLAG_C;
    }

    gb->a = result & 0xff;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t adc_a_b(Gameboy *gb){           // 0x88
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->b;
    
    uint16_t fc = 0;

    if (gb->f & FLAG_C){
        fc = 1;
    }

    uint16_t result = a + b + fc;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if ((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }

    if((a^b^result) & 0x10){
        gb->f |= FLAG_H;
    }

    if((result & 0x100) != 0){
        gb->f |= FLAG_C;
    }

    gb->a = result & 0xff;
    gb->pc++;
    return MCYCLE_1;
}


static gbcycles_t adc_a_c(Gameboy *gb){           // 0x89
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->c;
    
    uint16_t fc = 0;

    if (gb->f & FLAG_C){
        fc = 1;
    }

    uint16_t result = a + b + fc;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if ((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }

    if((a^b^result) & 0x10){
        gb->f |= FLAG_H;
    }

    if((result & 0x100) != 0){
        gb->f |= FLAG_C;
    }

    gb->a = result & 0xff;
    gb->pc++;
    return MCYCLE_1;
}


static gbcycles_t adc_a_e(Gameboy *gb){           // 0x8B
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->e;
    
    uint16_t fc = 0;

    if (gb->f & FLAG_C){
        fc = 1;
    }    

    uint16_t result = a + b + fc;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if ((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }
    
    if((a^b^result) & 0x10){
        gb->f |= FLAG_H;
    }

    if((result & 0x100) != 0){
        gb->f |= FLAG_C;
    }

    gb->a = result & 0xff;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t adc_a_h(Gameboy *gb){           // 0x8C
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->h;
    
    uint16_t fc = 0;

    if (gb->f & FLAG_C){
        fc = 1;
    }    

    uint16_t result = a + b + fc;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if ((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }
    
    if((a^b^result) & 0x10){
        gb->f |= FLAG_H;
    }

    if((result & 0x100) != 0){
        gb->f |= FLAG_C;
    }

    gb->a = result & 0xff;
    gb->pc++;
    return MCYCLE_1;
}


static gbcycles_t adc_a_mem_hl(Gameboy *gb){     // 0x8E
    uint16_t hl = (uint16_t)gb->h << 8 | (uint16_t)gb->l;
    uint8_t b = (uint8_t)gb->read(gb, hl);
    uint16_t a = (uint16_t)gb->a;
    uint16_t fc = 0;

    if (gb->f & FLAG_C){
        fc = 1;
    }

    uint16_t result = a + b + fc;

    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if ((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }

    if((a^b^result) & 0x10){
        gb->f |= FLAG_H;
    }

    if((result & 0x100) != 0){
        gb->f |= FLAG_C;
    }

    gb->a = result & 0xff;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t sub_c(Gameboy *gb){            // 0x91
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->c;
    uint16_t result = a - b;
    gb->f &= ~(FLAG_Z  | FLAG_H | FLAG_C);
    gb->f |= FLAG_N;

    if((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }

    if((a^b^result) & 0x10){
        gb->f |= FLAG_H;
    }

    if(result & 0x100){
        gb->f |= FLAG_C;
    }

    gb->a = result & 0xff;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t sub_l(Gameboy *gb){            // 0x95
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->l;
    uint16_t result = a - b;
    gb->f &= ~(FLAG_Z | FLAG_H | FLAG_C);
    gb->f |= FLAG_N;

    if((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }

    if((a^b^result) & 0x10){
        gb->f |= FLAG_H;
    }

    if(result & 0x100){
        gb->f |= FLAG_C;
    }

    gb->a = result & 0xff;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t sbc_a_d(Gameboy *gb){            // 0x9A
    
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->d;
    
    uint16_t fc = 0;

    if (gb->f & FLAG_C){
        fc = 1;
    }

    uint16_t result = a - b - fc;

    gb->f &= ~(FLAG_Z  | FLAG_H | FLAG_C);
    gb->f |= FLAG_N;

    if ((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }

    if((a^b^result) & 0x10){
        gb->f |= FLAG_H;
    }

    if(result & 0x100){
        gb->f |= FLAG_C;
    }

    gb->a = result & 0xff;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t sbc_a_e(Gameboy *gb){            // 0x9B
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->e;
    
    uint16_t fc = 0;

    if (gb->f & FLAG_C){
        fc = 1;
    }

    uint16_t result = a - b - fc;

    gb->f &= ~(FLAG_Z  | FLAG_H | FLAG_C);
    gb->f |= FLAG_N;

    if ((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }

    if((a^b^result) & 0x10){
        gb->f |= FLAG_H;
    }

    if(result & 0x100){
        gb->f |= FLAG_C;
    }

    gb->a = result & 0xff;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t and_c(Gameboy *gb){            // 0xA1
    uint8_t result = gb->a & gb->c;
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);
    gb->f |= FLAG_H;

    if(!result){gb->f |= FLAG_Z;}

    gb->a = result;
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

static gbcycles_t xor_b(Gameboy *gb){            // 0xA8
    gb->a ^= gb->b;
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if(!gb->a){gb->f |= FLAG_Z;}

    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t xor_c(Gameboy *gb){             // 0xA9
    gb->a ^= gb->c;
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if(!gb->a){gb->f |= FLAG_Z;}

    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t xor_h(Gameboy *gb){            // 0xAC
    gb->a ^= gb->h;
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if(!gb->a){gb->f |= FLAG_Z;}

    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t xor_hl_mm(Gameboy *gb){         // 0xAE
    uint16_t hl = (uint16_t)gb->h << 8 | (uint16_t)gb->l;
    uint8_t b = (uint8_t)gb->read(gb, hl);
    uint8_t result = gb->a ^ b;
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if(!result){gb->f |= FLAG_Z;}

    gb->a = result;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t xor_a(Gameboy *gb){             // 0xAF
    // uint8_t result = gb->a ^ gb->a;
    gb->a ^= gb->a;
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    gb->f |= FLAG_Z; // XOR'ing a value with itself always results in 0

    gb->pc++;
    return MCYCLE_1;
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


static gbcycles_t or_l(Gameboy *gb){            // 0xB5
    gb->a |= gb->l;
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if(!gb->a){gb->f |= FLAG_Z;}

    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t or_mem_hl(Gameboy *gb){        // 0xB6
    uint16_t hl = (uint16_t)gb->h << 8 | (uint16_t)gb->l;
    uint8_t b = (uint8_t)gb->read(gb, hl);
    uint8_t result = gb->a | b;
    gb->f &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);

    if(!result){gb->f |= FLAG_Z;}

    gb->a = result;
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t cp_b(Gameboy *gb){            // 0xB8
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->b;
    uint16_t result = a - b;
    gb->f &= ~(FLAG_Z  | FLAG_H | FLAG_C);
    gb->f |= FLAG_N;
    if((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }

    if((a^b^result) & 0x10){
        gb->f |= FLAG_H;
    }

    if(result & 0x100){
        gb->f |= FLAG_C;
    }
    
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t cp_d(Gameboy *gb){            // 0xBA
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->d;
    uint16_t result = a - b;
    gb->f &= ~(FLAG_Z  | FLAG_H | FLAG_C);
    gb->f |= FLAG_N;
    if((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }

    if((a^b^result) & 0x10){
        gb->f |= FLAG_H;
    }

    if(result & 0x100){
        gb->f |= FLAG_C;
    }
    
    gb->pc++;
    return MCYCLE_1;
}


static gbcycles_t cp_l(Gameboy *gb){            // 0xBD
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->l;
    uint16_t result = a - b;
    gb->f &= ~(FLAG_Z  | FLAG_H | FLAG_C);
    gb->f |= FLAG_N;
    if((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }

    if((a^b^result) & 0x10){
        gb->f |= FLAG_H;
    }

    if(result & 0x100){
        gb->f |= FLAG_C;
    }
    
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

    if(!(gb->f & FLAG_Z)){
        uint16_t call_address = gb->read(gb, gb->pc+2) << 8 | gb->read(gb, gb->pc+1);
        uint16_t sph = (gb->pc+3) >> 8;
        uint16_t spl = (gb->pc+3) & 0xFF;

        gb->write(gb, gb->sp-1, sph);
        gb->write(gb, gb->sp-2, spl);
        gb->sp -= 2;
        gb->pc = call_address;
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

static gbcycles_t rst_00h(Gameboy *gb){          // 0xC7
    uint16_t pch = (gb->pc+1) >> 8;
    uint16_t pcl = (gb->pc+1) & 0xFF;

    uint16_t sp1 = gb->sp - 1;
    uint16_t sp2 = gb->sp - 2;

    gb->write(gb, sp1, pch);
    gb->write(gb, sp2, pcl);
    gb->sp -= 2;
    gb->pc = 0x00;
    
    return MCYCLE_4;
}

static gbcycles_t ret_z(Gameboy *gb){            // 0xC8
    if(gb->f & FLAG_Z){
        uint16_t sph = gb->read(gb, gb->sp+1);
        uint16_t spl = gb->read(gb, gb->sp);
        gb->sp += 2;
        gb->pc = (sph << 8) | spl;
        return MCYCLE_4;
    }
    gb->pc++;
    return MCYCLE_1;
}

static gbcycles_t ret(Gameboy *gb){                // 0xC9
    uint16_t sph = gb->read(gb, gb->sp+1);
    uint16_t spl = gb->read(gb, gb->sp);
    gb->sp += 2;
    gb->pc = (sph << 8) | spl;

    return MCYCLE_4;
}

static gbcycles_t call_z_nn(Gameboy *gb){         // 0xCC
    if(gb->f & FLAG_Z){
        uint16_t call_address = gb->read(gb, gb->pc+2) << 8 | gb->read(gb, gb->pc+1);
        uint16_t sph = (gb->pc+3) >> 8;
        uint16_t spl = (gb->pc+3) & 0xFF;

        gb->write(gb, gb->sp-1, sph);
        gb->write(gb, gb->sp-2, spl);
        gb->sp -= 2;
        gb->pc = call_address;
        return MCYCLE_6;
    }
    gb->pc += 3;
    return MCYCLE_3;
}

static gbcycles_t call_nn(Gameboy *gb){            // 0xCD

    uint16_t call_address = gb->read(gb, gb->pc+2) << 8 | gb->read(gb, gb->pc+1);
    uint16_t sph = (gb->pc+3) >> 8;
    uint16_t spl = (gb->pc+3) & 0xFF;

    gb->write(gb, gb->sp-1, sph);
    gb->write(gb, gb->sp-2, spl);
    gb->sp -= 2;
    gb->pc = call_address;

    return MCYCLE_6;
}

static gbcycles_t jp_nc_nn(Gameboy *gb){         // 0xD2
    if(!(gb->f & FLAG_C)){
        uint16_t address = gb->read(gb, gb->pc+2) << 8 | gb->read(gb, gb->pc+1);
        gb->pc = address;
        return MCYCLE_4;
    }
    gb->pc += 3;
    return MCYCLE_3;
}

static gbcycles_t call_c_nn(Gameboy *gb){        // 0xDC

    if(gb->f & FLAG_C){
        uint16_t call_address = gb->read(gb, gb->pc+2) << 8 | gb->read(gb, gb->pc+1);
        uint16_t sph = (gb->pc+3) >> 8;
        uint16_t spl = (gb->pc+3) & 0xFF;

        gb->write(gb, gb->sp-1, sph);
        gb->write(gb, gb->sp-2, spl);
        gb->sp -= 2;
        gb->pc = call_address;
        return MCYCLE_6;
    }
    gb->pc += 3;
    return MCYCLE_3;
}

static gbcycles_t sbc_a_n(Gameboy *gb){          // 0xDE
    uint16_t a = (uint16_t)gb->a;
    uint16_t b = (uint16_t)gb->read(gb, gb->pc+1);

    uint16_t fc = 0;

    if (gb->f & FLAG_C){
        fc = 1;
    }

    uint16_t result = a - b - fc;

    gb->f &= ~(FLAG_Z | FLAG_H | FLAG_C);
    gb->f |= FLAG_N;

    if ((result & 0xff) == 0){
        gb->f |= FLAG_Z;
    }

    if((a^b^result) & 0x10){
        gb->f |= FLAG_H;
    }

    if(result & 0x100){
        gb->f |= FLAG_C;
    }

    gb->a = result & 0xff;
    gb->pc += 2;
    return MCYCLE_2;
}


static gbcycles_t rst_18h(Gameboy *gb){          // 0xDF
    uint16_t pch = (gb->pc+1) >> 8;
    uint16_t pcl = (gb->pc+1) & 0xFF;

    uint16_t sp1 = gb->sp - 1;
    uint16_t sp2 = gb->sp - 2;

    gb->write(gb, sp1, pch);
    gb->write(gb, sp2, pcl);
    gb->sp -= 2;
    gb->pc = 0x18;
    
    return MCYCLE_4;
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

static gbcycles_t ld_a_c_mem(Gameboy *gb){         // 0xF2
    uint8_t value = gb->c;
    gb->a = gb->read(gb, _IO + value);
    gb->pc++;
    return MCYCLE_2;
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
    gb->f |= FLAG_N;

    if(!result){gb->f |= FLAG_Z;}

    if((a^b^result) & 0x10){gb->f |= FLAG_H;}

    if ((result & 0x100) != 0){gb->f |= FLAG_C;}    

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
    [0x2E] = &ld_l_n,
    [0x2F] = &cpl,
    [0x30] = &jr_nc_i8,
    [0x31] = &ld_sp_nn,
    [0x32] = &ld_hlm_a,
    [0x33] = &inc_sp,
    [0x34] = &inc_mem_hl,
    [0x35] = &dec_mem_hl,
    [0x36] = &ld_mem_hl_n,
    [0x37] = &scf,
    [0x38] = &jr_c_i8,
    [0x39] = &add_hl_sp,
    [0x3A] = &ld_a_mem_hl_dec,
    [0x3B] = &dec_sp,
    [0x3C] = &inc_a,
    [0x3D] = &dec_a,
    [0x3E] = &ld_a_n,
    [0x3F] = &ccf,
    [0x40] = &ld_b_b,
    [0x41] = &ld_b_c,
    [0x42] = &ld_b_d,
    [0x43] = &ld_b_e,
    [0x44] = &ld_b_h,
    [0x45] = &ld_b_l,
    [0x46] = &ld_b_mem_hl,
    [0x47] = &ld_b_a,
    [0x48] = &ld_c_b,
    [0x49] = &ld_c_c,
    [0x4A] = &ld_c_d,
    [0x4B] = &ld_c_e,
    [0x4C] = &ld_c_h,
    [0x4D] = &ld_c_l,
    [0x4E] = &ld_c_mem_hl,
    [0x4F] = &ld_c_a,
    [0x50] = &ld_d_b,
    [0x51] = &ld_d_c,
    [0x52] = &ld_d_d,
    [0x53] = &ld_d_e,
    [0x54] = &ld_d_h,
    [0x55] = &ld_d_l,
    [0x56] = &ld_d_mem_hl,
    [0x57] = &ld_d_a,
    [0x58] = &ld_e_b,
    [0x59] = &ld_e_c,
    [0x5A] = &ld_e_d,
    [0x5B] = &ld_e_e,
    [0x5C] = &ld_e_h,
    [0x5D] = &ld_e_l,
    [0x5E] = &ld_e_mem_hl,
    [0x5F] = &ld_e_a,
    [0x60] = &ld_h_b,
    [0x61] = &ld_h_c,
    [0x62] = &ld_h_d,
    [0x63] = &ld_h_e,
    [0x64] = &ld_h_h,
    [0x65] = &ld_h_l,
    [0x66] = &ld_h_mem_hl,
    [0x67] = &ld_h_a,
    [0x68] = &ld_l_b,
    [0x69] = &ld_l_c,
    [0x6A] = &ld_l_d,
    [0x6B] = &ld_l_e,
    [0x6C] = &ld_l_h,
    [0x6D] = &ld_l_l,
    [0x6E] = &ld_l_mem_hl,
    [0x6F] = &ld_l_a,
    [0x70] = &ld_mem_hl_b,
    [0x71] = &ld_mem_hl_c,
    [0x72] = &ld_mem_hl_d,
    [0x73] = &ld_mem_hl_e,
    [0x74] = &ld_mem_hl_h,
    [0x75] = &ld_mem_hl_l,
    [0x76] = NULL,          // halt
    [0x77] = &ld_mem_hl_a,
    [0x78] = &ld_a_b,
    [0x79] = &ld_a_c,
    [0x7A] = &ld_a_d,
    [0x7B] = &ld_a_e,
    [0x7C] = &ld_a_h,
    [0x7D] = &ld_a_l,
    [0x7E] = &ld_a_mem_hl,
    [0x7F] = &ld_a_a,
    [0x80] = &add_a_b,
    [0x81] = &add_a_c,
    [0x82] = &add_a_d,
    [0x83] = &add_a_e,
    [0x84] = &add_a_h,
    [0x85] = &add_a_l,
    [0x86] = &add_a_mem_hl,
    [0x87] = &add_a_a,
    [0x88] = &adc_a_b,
    [0x89] = &adc_a_c,
    [0x8A] = NULL,
    [0x8B] = &adc_a_e,
    [0x8C] = &adc_a_h,
    [0x8D] = NULL,
    [0x8E] = &adc_a_mem_hl,
    [0x8F] = NULL,
    [0x90] = NULL,
    [0x91] = &sub_c,
    [0x92] = NULL,
    [0x93] = NULL,
    [0x94] = NULL,
    [0x95] = &sub_l,
    [0x96] = NULL,
    [0x97] = NULL,
    [0x98] = NULL,
    [0x99] = NULL,
    [0x9A] = &sbc_a_d,
    [0x9B] = &sbc_a_e,
    [0x9C] = NULL,
    [0x9D] = NULL,
    [0x9E] = NULL,
    [0x9F] = NULL,
    [0xA0] = NULL,
    [0xA1] = &and_c,
    [0xA2] = NULL,
    [0xA3] = &and_e,
    [0xA4] = NULL,
    [0xA5] = NULL,
    [0xA6] = NULL,
    [0xA7] = NULL,
    [0xA8] = &xor_b,
    [0xA9] = &xor_c,
    [0xAA] = NULL,
    [0xAB] = NULL,
    [0xAC] = &xor_h,
    [0xAD] = NULL,
    [0xAE] = &xor_hl_mm,
    [0xAF] = &xor_a,
    [0xB0] = NULL,
    [0xB1] = &or_c,
    [0xB2] = NULL,
    [0xB3] = NULL,
    [0xB4] = NULL,
    [0xB5] = &or_l,
    [0xB6] = &or_mem_hl,
    [0xB7] = NULL,
    [0xB8] = &cp_b,
    [0xB9] = NULL,
    [0xBA] = &cp_d,
    [0xBB] = NULL,
    [0xBC] = NULL,
    [0xBD] = &cp_l,
    [0xBE] = NULL,
    [0xBF] = NULL,
    [0xC0] = NULL,
    [0xC1] = &pop_bc,
    [0xC2] = &jp_nz_nn,
    [0xC3] = &jp_nn,
    [0xC4] = &call_nz_nn,
    [0xC5] = &push_bc,
    [0xC6] = NULL,
    [0xC7] = &rst_00h,
    [0xC8] = &ret_z,
    [0xC9] = &ret,
    [0xCA] = NULL,
    [0xCB] = NULL,
    [0xCC] = &call_z_nn,
    [0xCD] = &call_nn,
    [0xCE] = NULL,
    [0xCF] = NULL,
    [0xD0] = NULL,
    [0xD1] = NULL,
    [0xD2] = &jp_nc_nn,
    [0xD3] = NULL,
    [0xD4] = NULL,
    [0xD5] = NULL,
    [0xD6] = NULL,
    [0xD7] = NULL,
    [0xD8] = NULL,
    [0xD9] = NULL,
    [0xDA] = NULL,
    [0xDB] = NULL,
    [0xDC] = &call_c_nn,
    [0xDD] = NULL,
    [0xDE] = &sbc_a_n,
    [0xDF] = &rst_18h,
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
    [0xF2] = &ld_a_c_mem,
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
