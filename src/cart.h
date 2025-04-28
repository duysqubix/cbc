#ifndef ROM_H
#define ROM_H

#include <stdint.h>

#define ROM_BANK_SIZE 1024*16
#define RAM_BANK_SIZE 1024*8

typedef enum MBCType {
    ROM_ONLY = 0x00, MBC1 = 0x01, MBC1_RAM = 0x02, 
    MBC1_RAM_BATTERY = 0x03, MBC2 = 0x05, 
    MBC2_BATTERY = 0x06, ROM_RAM = 0x08, ROM_RAM_BATTERY = 0x09, 
    MMM01 = 0x0B, MMM01_RAM = 0x0C, MMM01_RAM_BATTERY = 0x0D, 
    MBC3_TIMER_BATTERY = 0x0F, MBC3_TIMER_RAM_BATTERY = 0x10, 
    MBC3 = 0x11, MBC3_RAM = 0x12, MBC3_RAM_BATTERY = 0x13, 
    MBC5 = 0x19, MBC5_RAM = 0x1A, MBC5_RAM_BATTERY = 0x1B, 
    MBC5_RUMBLE = 0x1C, MBC5_RUMBLE_RAM = 0x1D, MBC5_RUMBLE_RAM_BATTERY = 0x1E, 
    MBC6 = 0x20, MBC7_SENSOR_RUMBLE_RAM_BATTERY = 0x22, 
    POCKET_CAMERA = 0xFC, BANDAI_TAMA5 = 0xFD, 
    HUC3 = 0xFE, HUC1_RAM_BATTERY = 0xFF
} MBCType;


typedef struct Cartridge {
    MBCType  mbc_type;
    uint8_t *rom;
    uint16_t rom_bank_count;
    uint16_t rom_bank_select;

    uint8_t *ram;
    uint16_t ram_bank_count;
    uint16_t ram_bank_select;

    char    title[17];


    //public methods 
    uint8_t (*read)(struct Cartridge *self, uint16_t address);
    void    (*write)(struct Cartridge *self, uint16_t address, uint8_t value);
    int     (*load_rom)(struct Cartridge *self, const char *filename);

    // private methods
    void (*free)(struct Cartridge *self);

}Cartridge;

extern Cartridge *new_cartridge();

#endif