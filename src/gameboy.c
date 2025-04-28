#include <stddef.h>
#include <stdlib.h>
#include "gameboy.h"
#include "log.h"
#include "opcodes.h"
static void gameboy_free(Gameboy *self);
static uint8_t gameboy_read(Gameboy *self, uint16_t address);
static void gameboy_write(Gameboy *self, uint16_t address, uint8_t value);
static gbcycles_t gameboy_tick(Gameboy *self);

Gameboy *gameboy_new(const char *rom_filename){
    Gameboy *gb = (Gameboy *)malloc(sizeof(Gameboy));
    if (!gb){
        return NULL;
    }


    gb->cartridge = new_cartridge();
    gb->cartridge->load_rom(gb->cartridge, rom_filename);

    if (!gb->cartridge){
        free(gb);
        return NULL;
    }


    // assign public methods 
    gb->free = gameboy_free;
    gb->read = gameboy_read;
    gb->write = gameboy_write;
    gb->tick = gameboy_tick;
    return gb;
}

void gameboy_free(Gameboy *self){
    if (self->cartridge){
        log_trace("Freeing Cartridge");
        self->cartridge->free(self->cartridge);
    }

    if (self){
        log_trace("Freeing Gameboy");
        free(self);
    }
}

static gbcycles_t gameboy_tick(Gameboy *self){
    gbcycles_t  cycles = 0;
    size_t      offset = 0;
    uint8_t opcode = self->read(self, self->pc);

    if (opcode == 0xCB){
        self->pc++;
        opcode = self->read(self, self->pc);
        offset = 0x100;
    }

    opcode_def_t *op = opcodes[opcode + offset];
    if (!op){
        log_error("Invalid opcode: %02X", opcode);
        self->free(self);
        exit(1);
    }
    cycles += op(self);
    self->pc++;

    // cycles += opcodes[self->pc](self);
    // tick the cpu 
    // tick the cartridge 
    // tick the timer 
    // tick the lcd 
    // handle interrupts
    return 0;
}

GameboyState gameboy_run_until_complete(Gameboy *gb){
    log_info("Running Gameboy");

    if (!gb){
        log_error("Gameboy is NULL");
        return GAMEBOY_ERROR;
    }
    static gbcycles_t cycles = 0;

    while(1){
        cycles += gb->tick(gb);

    }
    return GAMEBOY_ERROR;
}


static uint8_t gameboy_read(Gameboy *self, uint16_t address){
    Cartridge *cart = self->cartridge;

    switch(address){
        case 0x0000 ... 0x7FFF:
            return cart->read(cart, address);
        case 0xA000 ... 0xBFFF:
            return cart->read(cart, address);

        case 0xE000 ... 0xFDFF:
            return self->memory[address - 0xE000];
        
        case 0xFFFF:
            return self->ie;

        default:
            return self->memory[address];
    }
}

static void gameboy_write(Gameboy *self, uint16_t address, uint8_t value){
    Cartridge *cart = self->cartridge;

    switch(address){
        case 0x0000 ... 0x7FFF:
            cart->write(cart, address, value);
            break;
        case 0xA000 ... 0xBFFF:
            cart->write(cart, address, value);
            break;
        case 0xE000 ... 0xFDFF:
            self->memory[address - 0xE000] = value;
            break;
        case 0xFFFF:
            self->ie = value;
            break;
        default:
            self->memory[address] = value;
            break;
    }
}