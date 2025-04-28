#ifndef GAMEBOY_H
#define GAMEBOY_H

#include <stdint.h>
#include "cart.h"

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


extern Gameboy *gameboy_new(const char *rom_filename);
extern GameboyState gameboy_run_until_complete(Gameboy *gb);
#endif
