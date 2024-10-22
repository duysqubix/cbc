#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> 
#include <time.h>
#include <string.h>

#include "defs.h"
#include "gameboy.h"
#include "opcodes.h"

/*-------------------- UTILS --------------------------- */

long read_file_into_buffer(const char *filename, uint8_t *buffer, long buffer_size) {
    long file_size;
    FILE *file;
    file = fopen(filename, "rb");

    if (file == NULL) {
        printf("Error: File not found\n");
        return -1;
    }

    // get the file size
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    // check if buffer is large enough
    if (buffer == NULL) {
        printf("Error: Buffer is NULL\n");
        fclose(file);
        return -1;
    }


    if (buffer_size < file_size) {
        printf("Error: Buffer is too small\n");
        printf("File size: %ld\n", file_size);
        printf("Buffer size: %ld\n", buffer_size);
        fclose(file);
        return -1;
    }

    // read the file into the buffer
    size_t result = fread(buffer, 1, file_size, file);
    if (result != file_size){
        printf("Error: Reading file failed\n");
        fclose(file);
        free(buffer);
        return -1;
    }  
    fclose(file);
    return file_size;
}


uint8_t* create_buffer8(uint32_t size, bool randomize) {
    uint8_t *ptr = (uint8_t *)calloc(size, sizeof(uint8_t));

    if (randomize){
        // randomize the buffer
        for (int i = 0; i < size; i++) {
            ptr[i] = (uint8_t)(rand() % 256);
        }
    }

    return ptr;
}

/*-------------------- GAMEBOY FUNCTIONS --------------------------- */
void gameboy_init(GAMEBOY *gb, const char *filename) {
    srand(time(NULL));

    init_cpu(gb);


    uint32_t total_banks = 200;
    uint32_t total_rom_size = GAMEBOY_ROM_BANK_SIZE * total_banks;
    uint32_t total_sram_size = GAMEBOY_RAM_BANK_SIZE * total_banks;
    uint32_t total_vram_size = GAMEBOY_VRAM_BANK_SIZE * 2; //max two banks
    uint32_t total_wram_size = GAMEBOY_RAM_BANK_SIZE * 8; // 8 kb

    gb->rom = create_buffer8(total_rom_size, false); // 200 banks of 16 kb
    gb->sram = create_buffer8(total_sram_size, true); // 200 banks of 8 kb
    gb->vram = create_buffer8(total_vram_size, true); // 8 kb

    gb->current_sram_bank = 0;
    gb->current_wram_bank = 0;
    gb->current_rom_bank = 0;
    gb->current_vram_bank = 0;

    read_file_into_buffer(filename, gb->rom, total_rom_size);
}

void gameboy_status(GAMEBOY *gb) {
    // get binary rep of F
    // create a buffer to store string representation of gameboy status 
    char buffer[1024];
    char *f_binary = display_register(gb->f);
    if (f_binary == NULL) {
        printf("Error: Memory allocation failed\n");
        return;
    }

    sprintf(buffer,
            "\nA: %02X\t F:%02X (%s)\n"
            "B: %02X\t C:%02X\n"
            "D: %02X\t E:%02X\n"
            "H: %02X\t L:%02X\n"
            "SP: %04X\t PC:%04X\n",
            gb->a, gb->f, f_binary,
            gb->b, gb->c,
            gb->d, gb->e,
            gb->h, gb->l,
            gb->sp, gb->pc);

    // get the next 10 elements in ROM based on current PC
    Register16 *pc = &gb->pc;
    for (int i = 0; i < 10; i++) {
        sprintf(buffer + strlen(buffer), "%02X ", gb->rom[*pc + i]);
    }

    printf("%s\n", buffer);
    free(f_binary);
}


char* display_register(uint8_t reg) {
    // create a buffer to store the binary representation of the register
    char *buffer = (char *)calloc(10, sizeof(char));

    if (buffer == NULL) {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }

    for (int i = 7; i >= 0; i--) {

        buffer[7 - i + (i < 4)] = (reg >> i) & 1 ? '1' : '0';
        if (i == 4) {
            buffer[4] = '_';
        }
    }

    buffer[9] = '\0';
    return buffer;
}


void gameboy_memset(GAMEBOY *gb, uint16_t addr, uint8_t val) {
    
    // ROM0
    if (0x0000 <= addr && addr < 0x4000) {
        return;
    }
    // ROMX 
    if (0x4000 <= addr && addr < 0x8000) {
        return;
    }
    
    // VRAM
    if (0x8000 <= addr && addr < 0xA000) {
        gb->vram[addr - 0x8000 + (gb->current_vram_bank * GAMEBOY_VRAM_BANK_SIZE)] = val;
        return;
    } 
    // SRAMX
    if (0xA000 <= addr && addr < 0xC000) {
        gb->sram[addr - 0xA000 + (gb->current_sram_bank * GAMEBOY_RAM_BANK_SIZE)] = val;
        return;
    }

    // WRAM0
    if (0xC000 <= addr && addr < 0xD000) {
        gb->wram[addr - 0xC000] = val;
        return;
    }

    // WRAMX
    if (0xD000 <= addr && addr < 0xE000) {
        gb->wram[addr - 0xD000 + (MAX(gb->current_wram_bank, 1) * GAMEBOY_RAM_BANK_SIZE)] = val;
        return;
    }

    // ECHO
    if (0xE000 <= addr && addr < 0xFE00) {
        gb->wram[addr - 0xE000 - 0xC000 + (MAX(gb->current_wram_bank, 1) * GAMEBOY_RAM_BANK_SIZE)] = val;
        return; 
    }

    // OAM
    if (0xFE00 <= addr && addr < 0xFEA0) {
        gb->oam[addr - 0xFE00] = val;
        return;
    }
    // UNUSED
    // IO

    if (0xFF00 <= addr && addr < 0xFF80) {
        gb->io[addr - 0xFF00] = val;
        return;
    }

    // HRAM
    if (0xFF80 <= addr && addr < 0xFFFF) {
        gb->hram[addr - 0xFF80] = val;
        return;
    }

    // IE
    if (addr == 0xFFFF) {
        gb->ime = val;
        return;
    }

    printf("Error: Address %04X out of bounds\n", addr);
    exit(1);
}

void gameboy_free(GAMEBOY *gb) {
    if (gb -> rom != NULL) {
        free(gb->rom);
    }
    gb->rom = NULL;

    if (gb->vram != NULL) {
        free(gb->vram);
    }

    gb->vram = NULL;

    if (gb->sram != NULL) {
        free(gb->sram);
    }

    gb->sram = NULL;


}


/*-------------------- CPU FUNCTIONS --------------------------- */
void init_cpu(GAMEBOY *gb) {
    if (gb->cgb_mode) {
        gb->a = 0x11;
        gb->f = 0b10000000;
        gb->b = 0x00;
        gb->c = 0x00;
        gb->d = 0xFF;
        gb->e = 0x56;
        gb->h = 0x00;
        gb->l = 0x0D;
        gb->sp = 0xFFFE;
        gb->pc = 0x0100;
    } else {
        gb->a = 0x01;
        gb->f = 0b10000000;
        gb->b = 0x00;
        gb->c = 0x13;
        gb->d = 0x00;
        gb->e = 0xD8;
        gb->h = 0x01;
        gb->l = 0x4D;
        gb->sp = 0xFFFE;
        gb->pc = 0x0100;
    }
}


