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


#define MCYCLE_1 1 
#define MCYCLE_2 2
#define MCYCLE_3 3
#define MCYCLE_4 4

#define AF() (uint16_t)(REG_A << 8 | REG_F)
#define BC() (uint16_t)(REG_B << 8 | REG_C)
#define DE() (uint16_t)(REG_D << 8 | REG_E)
#define HL() (uint16_t)(REG_H << 8 | REG_L)

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
#define INCR_SP() (REG_SP--)
#define DECR_SP() (REG_SP++)

#define BIT_SET(value, bit) (value |= (1 << bit))
#define BIT_RESET(value, bit) (value &= ~(1 << bit))
#define BIT_IS_SET(value, bit) (value & (1 << bit))

typedef uint8_t opcode_t;
typedef uint64_t opcycles_t;

extern uint8_t DISPLAY[DISPLAY_WIDTH*DISPLAY_HEIGHT];
extern uint8_t ROM[MAX_ROM_BANKS*ROM_BANK_SIZE];
extern uint8_t WRAM[MAX_RAM_BANKS*RAM_BANK_SIZE];
extern uint8_t SRAM[MAX_RAM_BANKS*RAM_BANK_SIZE];
extern uint8_t VRAM[MAX_VRAM_BANKS*RAM_BANK_SIZE];
extern uint8_t OAM[0x9F];
extern uint8_t IO [0x7F];
extern uint8_t HRAM[0x7E];
extern uint8_t IE;

extern uint16_t REG_SP;
extern uint16_t REG_PC;
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

extern uint8_t OPCODE_LENGTH[512];
extern char * const OPCODE_NAMES[512];
extern bool STEP_MODE;


void gameboy_init(const char *rom_path);
void gameboy_free();
int  gameboy_loop();
void randomize(uint8_t *data, size_t size);
opcycles_t execute_opcode(opcode_t opcode, uint16_t value);

// utilities 
void fdump_memory(const char *filename, uint8_t *data, size_t size);
void dump_registers();

#endif
