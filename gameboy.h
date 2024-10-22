#ifndef GAMEBOY_H
#define GAMEBOY_H

#include<stdint.h>
#include <stdbool.h>


typedef uint8_t Register;
typedef uint16_t Register16;
typedef uint8_t *Memory;

typedef uint8_t Bank;

typedef struct gameboy {
    Register a;
    Register f;
    Register b;
    Register c;
    Register d;
    Register e;
    Register h;
    Register l;
    Register16 sp;
    Register16 pc;

    Register ime;

    Memory rom;
    Memory vram;
    Memory sram;
    Memory wram;
    Memory oam;
    Memory io;
    Memory hram;

    Memory *mmap;

    Bank current_rom_bank;
    Bank current_sram_bank;
    Bank current_wram_bank;
    Bank current_vram_bank;

    bool cgb_mode;
} GAMEBOY;

long read_file_into_buffer(const char *filename, uint8_t *buffer, long size);
uint8_t* create_buffer8(uint32_t size, bool randomize);
void gameboy_init(GAMEBOY *gb, const char *filename);
void gameboy_status(GAMEBOY *gb);
void gameboy_free(GAMEBOY *gb);
uint8_t gameboy_memget(GAMEBOY *gb, uint16_t addr);
void gameboy_memset(GAMEBOY *gb, uint16_t addr, uint8_t val);

char *display_register(uint8_t reg);
void init_cpu(GAMEBOY *gb);
#endif 
