#ifndef GAMEBOY_H
#define GAMEBOY_H

#include <stdint.h>
#include "cart.h"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144
#define SCALE 4
#define WINDOW_WIDTH (SCREEN_WIDTH * SCALE)
#define WINDOW_HEIGHT (SCREEN_HEIGHT * SCALE)

#define FLAG_Z 0b10000000
#define FLAG_N 0b01000000
#define FLAG_H 0b00100000
#define FLAG_C 0b00010000

#define _VRAM 0x8000 
#define _SRAM 0xA000
#define _RAM 0xC000
#define _RAMBANK 0xD000
#define _OAM 0xFE00
#define _IO 0xFF00
#define _HRAM 0xFF80

#define _IO_LY 0xFF44


typedef uint64_t gbcycles_t;

typedef struct Gameboy {
    Cartridge *cartridge;
    uint8_t memory[0xFFFF];
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t f;
    uint8_t h;
    uint8_t l;
    uint16_t pc;
    uint16_t sp;
    uint8_t flags;
    uint8_t interrupt_enable;
    uint8_t ie;

    uint8_t halted;
    uint8_t is_stuck;

    uint32_t frame_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];
    uint8_t buttons;

    // public methods 
    void (*free)(struct Gameboy *self);
    uint8_t (*read)(struct Gameboy *self, uint16_t address);
    void (*write)(struct Gameboy *self, uint16_t address, uint8_t value);
    gbcycles_t (*tick)(struct Gameboy *self);

}Gameboy;

typedef enum GameboyState {
    GAMEBOY_RUNNING,
    GAMEBOY_ERROR,
    GAMEBOY_INVALID_OPCODE
}GameboyState;


typedef struct {
    struct { uint16_t address; uint8_t enabled;} step_at;
    uint8_t trace_state;
    const char *trace_file;

} gflags_t;

extern gflags_t gflags;


extern Gameboy *gameboy_new(const char *rom_filename);
extern GameboyState gameboy_run_until_complete(Gameboy *gb);
#endif
