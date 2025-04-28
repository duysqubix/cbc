#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cart.h"
#include "log.h"

static uint8_t cart_read(Cartridge *self, uint16_t address);
static void cart_write(Cartridge *self, uint16_t address, uint8_t value);
static int cart_load_rom(Cartridge *self, const char *filename);
static void free_cartridge(Cartridge *self);
static const char *get_mbc_type(MBCType mbc_type);
static const char *mbc_types[];

Cartridge *new_cartridge(){
    Cartridge *cart = (Cartridge *)malloc(sizeof(Cartridge));
    if (!cart){
        log_error("Failed to allocate memory for cartridge");
        return NULL;
    }

    // assign public methods 
    cart->read = cart_read;
    cart->write = cart_write;
    cart->load_rom = cart_load_rom;
    cart->free = free_cartridge;

    return cart;
}


void free_cartridge(Cartridge *self){
    if (self->rom){
        log_trace("Freeing rom memory");
        free(self->rom);
        self->rom = NULL;
    }
    if (self->ram){
        log_trace("Freeing ram memory");
        free(self->ram);
        self->ram = NULL;
    }

    if (self){
        free(self);
        self = NULL;
    }
}


 int cart_load_rom(Cartridge *self, const char *filename){
    FILE *file = fopen(filename, "rb");
    if (!file){
        log_error("Failed to open ROM: %s", filename);
        free_cartridge(self);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t buffer[size];
    int result = fread(buffer, 1, size, file);

    if (!result){
        log_error("Failed to read ROM: %s", filename);
        free_cartridge(self);
        exit(1);
    }

    fclose(file);


    strncpy(self->title, (char *)buffer + 0x134, 16);
    self->title[16] = '\0';

    uint32_t rom_size = 32*1024 * (1 << (buffer[0x148]));
    uint16_t rom_bank_count = (rom_size / (16 * 1024));

    self->rom = (uint8_t *)malloc(rom_size);
    if (!self->rom){
        log_error("Failed to allocate memory for ROM");
        free_cartridge(self);
        exit(1);
    }
    
    memcpy(self->rom, buffer, rom_size);
    
    uint8_t ram_id = buffer[0x149];
    uint16_t ram_size = 0;

    switch(ram_id){
        case 0x00:
            self->ram = NULL;
            self->ram_bank_count = 0;
            break;
        case 0x01:
            log_error("RAM size not supported");
            free_cartridge(self);
            return 0;
        case 0x02:
            self->ram = (uint8_t *)malloc(8*1024);
            self->ram_bank_count = (8*1024) / (8*1024);
            break;
        case 0x03:
            self->ram = (uint8_t *)malloc(32*1024);
            self->ram_bank_count = (32*1024) / (8*1024);
            break;
        case 0x04:
            self->ram = (uint8_t *)malloc(128*1024);
            self->ram_bank_count = (128*1024) / (8*1024);
            break;
        case 0x05:
            self->ram = (uint8_t *)malloc(64*1024);
            self->ram_bank_count = (64*1024) / (8*1024);
            break;
        default:
            log_error("RAM size not supported");
            free_cartridge(self);
            return 0;
    };

    self->mbc_type = (MBCType)buffer[0x0147];

    self->rom_bank_select = 1;
    self->ram_bank_select = 0;

    log_info("Loaded ROM: %s", filename);
    log_info("Title: %s", self->title);
    log_info("MBC Type: %s", get_mbc_type(self->mbc_type));
    log_info("ROM Size: %d bytes", rom_size);
    log_info("ROM Bank Count: %d x 16KiB", rom_bank_count);
    log_info("RAM Size: %d bytes", ram_size);
    log_info("RAM Bank Count: %d x 8KiB", self->ram_bank_count);

    return 1;
}

uint8_t cart_read(Cartridge *self, uint16_t address) {
    switch(address){
        case 0x0000 ... 0x3FFF:
            return self->rom[address];
        case 0x4000 ... 0x7FFF:
            return self->rom[address + (self->rom_bank_select * ROM_BANK_SIZE)];
        case 0xA000 ... 0xBFFF:
            if (self->ram){
                return self->ram[address - 0xA000];
            }
            return 0;
        default:
            return 0;
    }
}

void cart_write(Cartridge *self, uint16_t address, uint8_t value) {
    log_error("Write to cartridge not implemented");
    exit(1);
}

static const char *get_mbc_type(MBCType mbc_type){
    return mbc_types[mbc_type];
}

static const char *mbc_types[] = {
    [0x00] = "ROM ONLY",
    [0x01] = "MBC1",
    [0x02] = "MBC1+RAM",
    [0x03] = "MBC1+RAM+BATTERY",
    [0x05] = "MBC2",
    [0x06] = "MBC2+BATTERY",
    [0x08] = "ROM+RAM",
    [0x09] = "ROM+RAM+BATTERY",
    [0x0B] = "MMM01",
    [0x0C] = "MMM01+RAM",
    [0x0D] = "MMM01+RAM+BATTERY",
    [0x0F] = "MBC3+TIMER+BATTERY",
    [0x10] = "MBC3+TIMER+RAM+BATTERY",
    [0x11] = "MBC3",
    [0x12] = "MBC3+RAM",
    [0x13] = "MBC3+RAM+BATTERY",
    [0x19] = "MBC5",
    [0x1A] = "MBC5+RAM",
    [0x1B] = "MBC5+RAM+BATTERY",
    [0x1C] = "MBC5+RUMBLE",
    [0x1D] = "MBC5+RUMBLE+RAM",
    [0x1E] = "MBC5+RUMBLE+RAM+BATTERY",
    [0x20] = "MBC6",
    [0x22] = "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
    [0xFC] = "POCKET CAMERA",
    [0xFD] = "BANDAI TAMA5",
    [0xFE] = "HuC3",
    [0xFF] = "HuC1+RAM+BATTERY",
};



