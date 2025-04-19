
#include "cbc.h"

void randomize(uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        data[i] = rand() % 256;
    }
}

void gameboy_init(){
    gameboy.rom = (uint8_t *)malloc(128*ROM_BANK_SIZE);
    gameboy.wram = (uint8_t *)malloc(32*RAM_BANK_SIZE);
    gameboy.vram = (uint8_t *)malloc(2*RAM_BANK_SIZE);

    randomize(gameboy.wram, 32*RAM_BANK_SIZE);
    randomize(gameboy.vram, 2*RAM_BANK_SIZE);
    randomize(gameboy.hram, 0x7F);
}

void gameboy_free() {
    free(gameboy.rom);
    free(gameboy.wram);
    free(gameboy.vram);

    gameboy.rom = NULL;
    gameboy.wram = NULL;
    gameboy.vram = NULL;
}
