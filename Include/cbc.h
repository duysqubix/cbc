#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>
#include <stdlib.h>

#define ROM_BANK_SIZE 1024*16 // 16KB
#define RAM_BANK_SIZE 1024*8 // 8KB

#define REG_HIGH(reg) (uint8_t)(reg >> 8)
#define SET_REG_HIGH(reg)
#define REG_LOW(reg) (uint8_t)(reg & 0xFF)

typedef struct {
    uint8_t *rom;
    uint8_t *wram;
    uint8_t *vram;
} Gameboy;

static uint8_t oam[0xFEA0-0xFE00];
static uint8_t io[0xFF80-0xFF00];
static uint8_t hram[0xFFFF-0xFF80];
static uint8_t ie;

static uint16_t sp;
static uint16_t pc;
static uint8_t f;
static uint8_t a;
static uint8_t b;
static uint8_t c;
static uint8_t d;
static uint8_t e;
static uint8_t h;
static uint8_t l;

static Gameboy gameboy;


void gameboy_init();
void gameboy_free();
void randomize(uint8_t *data, size_t size);

#endif
