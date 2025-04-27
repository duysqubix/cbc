#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROM_BANK_SIZE   1024*16 // 16KB
#define RAM_BANK_SIZE   1024*8 // 8KB
#define DISPLAY_WIDTH   160
#define DISPLAY_HEIGHT  144
#define MAX_ROM_BANKS   128
#define MAX_RAM_BANKS   32
#define MAX_VRAM_BANKS  2

#define MAX_ROM_SIZE MAX_ROM_BANKS*ROM_BANK_SIZE
#define MAX_RAM_SIZE MAX_RAM_BANKS*RAM_BANK_SIZE
#define MAX_VRAM_SIZE MAX_VRAM_BANKS*RAM_BANK_SIZE

#define DISPLAY_SET(x, y, value) DISPLAY[(y)*DISPLAY_WIDTH + (x)] = value
#define DISPLAY_GET(x, y) DISPLAY[(y)*DISPLAY_WIDTH + (x)]

#define ROM_GET(bank, offset) ROM[(bank*ROM_BANK_SIZE) + offset]

#define WRAM_SET(bank, offset, value) WRAM[(bank*RAM_BANK_SIZE) + offset] = value
#define WRAM_GET(bank, offset) WRAM[(bank*RAM_BANK_SIZE) + offset]

#define SRAM_SET(bank, offset, value) SRAM[(bank*RAM_BANK_SIZE) + offset] = value
#define SRAM_GET(bank, offset) SRAM[(bank*RAM_BANK_SIZE) + offset]

#define VRAM_SET(bank, offset, value) VRAM[(bank*RAM_BANK_SIZE) + offset] = value
#define VRAM_GET(bank, offset) VRAM[(bank*RAM_BANK_SIZE) + offset]

#define U32(value) (uint32_t)(value)
#define U16(value) (uint16_t)(value)
#define U8(value) (uint8_t)(value)
#define i8(value) (int8_t)(value)

#define MCYCLE_1 1 
#define MCYCLE_2 2
#define MCYCLE_3 3
#define MCYCLE_4 4
#define MCYCLE_5 5
#define MCYCLE_6 6

#define AF() (U16(REG_A) << 8 | REG_F)
#define BC() (U16(REG_B) << 8 | REG_C)
#define DE() (U16(REG_D) << 8 | REG_E)
#define HL() (U16(REG_H) << 8 | REG_L)

#define FLAG_Z_SET()    (REG_F |=  0b10000000)
#define FLAG_Z_RESET()  (REG_F &= ~0b10000000)
#define FLAG_Z_IS_SET() (REG_F & 0b10000000)

#define FLAG_N_SET()    (REG_F |=  0b01000000)
#define FLAG_N_RESET()  (REG_F &= ~0b01000000)
#define FLAG_N_IS_SET() (REG_F & 0b01000000)

#define FLAG_H_SET()    (REG_F |=  0b00100000)
#define FLAG_H_RESET()  (REG_F &= ~0b00100000)
#define FLAG_H_IS_SET() (REG_F & 0b00100000)

#define FLAG_C_SET()    (REG_F |=  0b00010000)
#define FLAG_C_RESET()  (REG_F &= ~0b00010000)
#define FLAG_C_IS_SET() (REG_F & 0b00010000)


#define INCR_PC() (REG_PC++)

#define BIT_SET(value, bit) (value |= (1 << bit))
#define BIT_RESET(value, bit) (value &= ~(1 << bit))
#define BIT_IS_SET(value, bit) (value & (1 << bit))

#define WRITE_MEM(address, value) write_item((address_t)(address), U8(value))
#define READ_MEM(address) fetch_item((address_t)(address))

#define READ_NEXT_BYTE() U8(READ_MEM(REG_PC + 1))
#define READ_NEXT_WORD() U16(READ_MEM(REG_PC + 2) << 8 | READ_MEM(REG_PC + 1))

#define CP_SET_FLAGS(a, b)                                                  \
{                                                                           \
    FLAG_N_SET();                                                           \
    FLAG_H_RESET();                                                         \
    FLAG_C_RESET();                                                         \
    FLAG_Z_RESET();                                                         \
    if (((U16(a)-U16(b)) & (0xff)) == 0) { FLAG_Z_SET(); }                  \
    if (((U16(a)^U16(b)^(U16(a)-U16(b))) & (0x10)) != 0) { FLAG_H_SET(); }  \
    if (((U16(a)-U16(b)) & (0x100)) != 0) { FLAG_C_SET(); }                 \
}


#define AND_SET_FLAGS(a, b)                                                \
{                                                                          \
    uint8_t r = a & b;                                                      \
    FLAG_Z_RESET();                                                        \
    FLAG_N_RESET();                                                        \
    FLAG_H_RESET();                                                        \
    FLAG_C_RESET();                                                        \
    if (!r) { FLAG_Z_SET(); }                                              \
    a = r;                                                                 \
}

#define ADD_SET_FLAGS16(ahigh, alow, bhigh, blow)                           \
{                                                                           \
    uint16_t a = U16(ahigh) << 8 | U16(alow);                               \
    uint16_t b = U16(bhigh) << 8 | U16(blow);                               \
    uint16_t r = a + b;                                                     \
    FLAG_N_RESET();                                                         \
    FLAG_H_RESET();                                                         \
    FLAG_C_RESET();                                                         \
    if (r & 0x10000) { FLAG_C_SET(); }                                      \
    if ((a^b^r) & 0x1000) { FLAG_H_SET(); }                                 \
    ahigh = U8(r >> 8);                                                     \
    alow = U8(r & 0xFF);                                                    \
}

#define OR_SET_FLAGS(a, b)                          \
{                                                   \
    uint8_t r = a | b;                                  \
    FLAG_Z_RESET();                                     \
    if (!r) { FLAG_Z_SET(); }                           \
    FLAG_N_RESET();                                     \
    FLAG_H_RESET();                                     \
    FLAG_C_RESET();                                     \
    a = r;                                          \
}

#define INC_BC()                                    \
{                                                   \
    REG_C++;                                        \
    if (!REG_C) {                                   \
        REG_B++;                                    \
    }                                               \
}

#define DEC_BC()                                    \
{                                                   \
    REG_C--;                                        \
    if (REG_C == 0xFF){                             \
        REG_B--;                                    \
    }                                               \
}


#define INC_DE()                                    \
{                                                   \
    if (!(++REG_E)) {                               \
        REG_D++;                                    \
    }                                               \
}

#define DEC_DE()                                    \
{                                                   \
    REG_E--;                                        \
    if (REG_E == 0xFF){                             \
        REG_D--;                                    \
    }                                               \
}


#define INC_HL()                                    \
{                                                   \
    REG_L++;                                        \
    if (!(++REG_L)) {                               \
        REG_H++;                                    \
    }                                               \
}

#define DEC_HL()                                    \
{                                                   \
    REG_L--;                                        \
    if (REG_L == 0xFF){                             \
        REG_H--;                                    \
    }                                               \
}

typedef uint8_t opcode_t;
typedef uint64_t opcycles_t;
typedef uint16_t address_t;
typedef opcycles_t opcode_def_t();


extern uint8_t DISPLAY[DISPLAY_WIDTH*DISPLAY_HEIGHT];
extern uint8_t ROM[MAX_ROM_BANKS*ROM_BANK_SIZE];
extern uint8_t WRAM[MAX_RAM_BANKS*RAM_BANK_SIZE];
extern uint8_t SRAM[MAX_RAM_BANKS*RAM_BANK_SIZE];
extern uint8_t VRAM[MAX_VRAM_BANKS*RAM_BANK_SIZE];
extern uint8_t OAM[0x9F];
extern uint8_t IO [0x7F];
extern uint8_t HRAM[0x7E];
extern uint8_t IE;

extern address_t REG_SP;
extern address_t REG_PC;
extern uint8_t  REG_F;
extern uint8_t  REG_A;
extern uint8_t  REG_B;
extern uint8_t  REG_C;
extern uint8_t  REG_D;
extern uint8_t  REG_E;
extern uint8_t  REG_H;
extern uint8_t  REG_L;

extern uint8_t ROM_CURRENT_BANK;
extern uint8_t VRAM_CURRENT_BANK;
extern uint8_t SRAM_CURRENT_BANK;
extern uint8_t WRAM_CURRENT_BANK;

extern uint8_t const OPCODE_LENGTH[512];
extern char * const OPCODE_NAMES[512];
extern opcode_def_t *opcodes[512];

extern bool CPU_STUCK;
extern bool CPU_HALTED;
extern bool DEBUG_STEP_MODE;
extern uint16_t BREAK_INSTR;

extern uint8_t     fetch_item(address_t address);
extern void        write_item(address_t address, uint8_t value);


void gameboy_init(const char *rom_path);
void gameboy_free();
int  gameboy_loop();
void randomize(uint8_t *data, size_t size);
opcycles_t execute_opcode(opcode_t opcode, uint16_t value);
void dump_memory();

// utilities 
void fdump_memory(const char *filename, uint8_t *data, size_t size);
void dump_registers();




#endif
