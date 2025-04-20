
#include <stdio.h>
#include <time.h>
#include "defs.h"
#include "log.h"



opcycles_t  _tick();
opcycles_t  _tick_cpu(opcycles_t cycles);

uint8_t     _fetch_item(uint16_t address);
void        _write_item(uint16_t address, uint16_t value);
opcycles_t  _execute(opcode_t opcode, uint16_t value);

void randomize(uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        data[i] = rand() % 256;
    }
}

void _load_rom(const char *rom_path){
    FILE * rom = fopen(rom_path, "rb");
    if (!rom) {
        log_error("Failed to open ROM: %s", rom_path);
        exit(1);
    }

    fseek(rom, 0, SEEK_END);
    long size = ftell(rom);
    fseek(rom, 0, SEEK_SET);

    fread(ROM, 1, size, rom);
    fclose(rom);


    log_info("Loaded ROM: %s", rom_path);
    log_info("ROM Size: %d bytes", size);
    fdump_memory("rom.txt", ROM, size);
}

void gameboy_init(const char *rom_path){

    // Set Internals
    randomize(WRAM, MAX_RAM_SIZE);
    randomize(VRAM, MAX_VRAM_SIZE);
    randomize(SRAM, MAX_RAM_SIZE);

    REG_A = 0x11;
    FLAG_Z_SET();
    FLAG_N_RESET();
    FLAG_H_RESET();
    FLAG_C_RESET();
    REG_B = 0x00;
    REG_C = 0x00;
    REG_D = 0xFF;
    REG_E = 0x56;
    REG_H = 0x00;
    REG_L = 0x0D;
    REG_PC = 0x0100;
    REG_SP = 0xFFFE;

    _load_rom(rom_path);
}

int gameboy_loop(){

    while (1) {
        if (_tick()) {
            log_error("Error occured");
            break;
        }
    }
        // 
    return 0;
}

opcycles_t _tick(){
    static opcode_t opcode;
    static opcycles_t cycles = 4; 

    //update cpu 
    cycles = _tick_cpu(cycles);

    //update cartridge
    //update timers
    //update lcd
    // handle interrupts
    dump_registers();
    getchar();
    return 0;
}

opcycles_t _tick_cpu(opcycles_t cycles){
    opcycles_t opcycles = 0;
    opcode_t opcode = (opcode_t)_fetch_item(REG_PC);
    uint16_t value = 0;

    switch (OPCODE_LENGTH[opcode]) {
        case 1:
            INCR_PC();
            break;

        // 8bit immediate
        case 2:
            value = _fetch_item(REG_PC+1);
            INCR_PC();
            break;

        // 16bit immediate
        case 3:
            value = (_fetch_item(REG_PC+2) << 8) | _fetch_item(REG_PC+1);
            INCR_PC();
            INCR_PC();
            break;
    }

    log_trace("Opcode: %02X, Value: %04X", opcode, value);

    opcycles += _execute(opcode, value);

    return opcycles;
}

uint8_t _fetch_item(uint16_t address){
    log_trace("Fetching item at address: %04X", address);
    switch (address) {
        case 0x0000 ... 0x3FFF:
            return ROM_GET(0, address);

        case 0x4000 ... 0x7FFF:
            return ROM_GET(ROM_CURRENT_BANK, address - 0x4000);

        case 0x8000 ... 0x9FFF:
            return VRAM_GET(VRAM_CURRENT_BANK, address - 0x8000);

        case 0xA000 ... 0xBFFF:
            return SRAM_GET(SRAM_CURRENT_BANK, address - 0xA000);

        case 0xC000 ... 0xCFFF:
            return WRAM_GET(0, address - 0xC000);

        case 0xD000 ... 0xDFFF:
            return WRAM_GET(WRAM_CURRENT_BANK, address - 0xD000);

        case 0xE000 ... 0xEFFF:
            return WRAM_GET(0, address - 0xE000);

        case 0xF000 ... 0xFDFF:
            return WRAM_GET(WRAM_CURRENT_BANK, address - 0xF000);
        
        case 0xFE00 ... 0xFE9F:
            return OAM[address - 0xFE00];

        case 0xFEA0 ... 0xFEFF:
            return 0x00;

        case 0xFF00 ... 0xFF7F:
            return IO[address - 0xFF00];

        case 0xFF80 ... 0xFFFE:
            return HRAM[address - 0xFF80];

        case 0xFFFF:
            return IE;

        default:
            log_error("Invalid fetch address: %d", address);
            exit(1);       
    }
}

void _write_item(uint16_t address, uint16_t value){
    ROM[address] = value;
}

opcycles_t _execute(opcode_t opcode, uint16_t value){
    switch (opcode) {
        case 0x00:
            INCR_PC();
            return 1;
        case 0x06:
            REG_B = value;
            INCR_PC();
            return 2;

        case 0x37:
            FLAG_C_SET();
            return 1;
        default:
            log_error("Invalid opcode: %02X", opcode);
            exit(1);
    }
}