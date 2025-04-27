#include <stdio.h>
#include <time.h>
#include "defs.h"
#include "log.h"



int         _tick();
opcycles_t  _tick_cpu();
opcycles_t  _execute(opcode_t opcode, uint16_t value);
opcycles_t  _executeCB(opcode_t opcode);

bool CPU_HALTED = false;
bool CPU_STUCK = false;

void dump_registers(){
    if (get_log_level() > LOG_TRACE){ return; }

    char const * str = "\n"
        "A: $%02X\t\tF: %04b [znhc] [%02X]\n" 
        "B: $%02X\t\tC: $%02X\t\t[BC:$%04X]: $%02X\n" 
        "D: $%02X\t\tE: $%02X\t\t[DE:$%04X]: $%02X\n" 
        "H: $%02X\t\tL: $%02X\t\t[HL:$%04X]: $%02X\n" 
        "PC: $%04X\tSP: $%04X\n";

    log_trace(str, 
        REG_A, REG_F>>4, REG_F,
        REG_B, REG_C, BC(), READ_MEM(BC()),
        REG_D, REG_E, DE(), READ_MEM(DE()), 
        REG_H, REG_L, HL(), READ_MEM(HL()), 
        REG_PC, REG_SP);
    
}

size_t _load_rom(const char *rom_path){
    FILE * rom = fopen(rom_path, "rb");
    if (!rom) {
        log_error("Failed to open ROM: %s", rom_path);
        exit(1);
    }

    fseek(rom, 0, SEEK_END);
    long size = ftell(rom);
    fseek(rom, 0, SEEK_SET);

    size_t n = fread(ROM, 1, size, rom);
    fclose(rom);


    log_info("Loaded ROM: %s", rom_path);
    log_info("ROM Size: %d bytes", size);
    fdump_memory("rom.txt", ROM, size);
    return n;
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

    IO[0x44] = 145;

    _load_rom(rom_path);
}

int gameboy_loop(){
    size_t cycles = 0;
    size_t n = 0;
    size_t tick_count = 0;
    while (1) {
        log_trace("Tick Begin: %d", tick_count);
        n = _tick();
        if (!n) {
            log_error("Error occured");
            break;
        }
        cycles += n;
        log_trace("Tick End: %d | Cycle Total: %d", ++tick_count, cycles);
    }
    return 0;
}

int _tick(){
    opcycles_t cycles = 0; 
    // address_t old_pc = REG_PC;
    // address_t old_sp = REG_SP;
    
    //update cpu 
    cycles += _tick_cpu(cycles);
    
    // if (!CPU_HALTED && (old_pc == REG_PC) && (old_sp == REG_SP) && !CPU_STUCK){?
    if (CPU_STUCK){
        log_warn("CPU Stuck at PC: %04X, SP: %04X", REG_PC, REG_SP);
        getchar();
    }

    //update cartridge
    //update timers
    //update lcd
    // handle interrupts
    if (DEBUG_STEP_MODE) {
        getchar();
    }
    return cycles;
}

opcycles_t _tick_cpu(){
    opcode_t opcode = READ_MEM(REG_PC);
    uint16_t offset = 0x00;
    opcycles_t opcycles = 0;
    opcode_def_t *opcycle_def = NULL;

    if(opcode == 0xCB){
        offset = 0x100;
        REG_PC++;
        opcode = READ_MEM(REG_PC);
        opcycles++;
    }

    int8_t oplen = OPCODE_LENGTH[(size_t)opcode+offset];

    if (get_log_level() >= LOG_TRACE){
        switch(oplen){
            case 1:
                log_trace("Opcode: %02X[%d length], Value: N/a | %s", opcode, OPCODE_LENGTH[(size_t)opcode], OPCODE_NAMES[(size_t)opcode]);
                break;

            case 2:
                log_trace("Opcode: %02X[%d length], Value: %02X | %s", opcode, OPCODE_LENGTH[(size_t)opcode], READ_NEXT_BYTE(), OPCODE_NAMES[(size_t)opcode]);
                break;
                
            case 3:
                log_trace("Opcode: %02X[%d length], Value: %04X | %s", opcode, OPCODE_LENGTH[(size_t)opcode], READ_NEXT_WORD(), OPCODE_NAMES[(size_t)opcode]);
                break;
        }
    }


    opcycle_def = opcodes[opcode+offset];

    // TODO: remove me once all opcodes are implemented
    if(!opcycle_def){
        log_error("Invalid opcode: %02X", opcode);
        exit(1);
    }

    opcycles += opcycle_def();
    REG_PC   += OPCODE_LENGTH[opcode+offset];

    dump_registers();

    if (opcode == BREAK_INSTR){
        log_info("Break instruction: %04X", opcode);
        exit(0);
    }

    return opcycles;
}

uint8_t fetch_item(address_t address){
    // log_trace("Fetching item at address: %04X", address);
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

void write_item(address_t address, uint8_t value){
    // write serial to stdout 
    if (address == 0xFF02 && value == 0x81){
        printf("%c", IO[0x01]);
    }

    switch (address) {
        case 0x0000 ... 0x3FFF:
            // Write to Cartridge
            return;

        case 0x4000 ... 0x7FFF:
            // Write to Cartridge
            return;

        case 0x8000 ... 0x9FFF:
            VRAM_SET(VRAM_CURRENT_BANK, address - 0x8000, value);
            return;

        case 0xA000 ... 0xBFFF:
            SRAM_SET(SRAM_CURRENT_BANK, address - 0xA000, value);
            return;

        case 0xC000 ... 0xCFFF:
            WRAM_SET(0, address - 0xC000, value);
            return;

        case 0xD000 ... 0xDFFF:
            WRAM_SET(WRAM_CURRENT_BANK, address - 0xD000, value);
            return;

        case 0xE000 ... 0xEFFF:
            WRAM_SET(0, address - 0xE000, value);
            return;

        case 0xF000 ... 0xFDFF:
            WRAM_SET(WRAM_CURRENT_BANK, address - 0xF000, value);
            return;
        
        case 0xFE00 ... 0xFE9F:
            OAM[address - 0xFE00] = value;
            return;

        case 0xFEA0 ... 0xFEFF:
            return;

        case 0xFF00 ... 0xFF7F:
            IO[address - 0xFF00] = value;
            return;

        case 0xFF80 ... 0xFFFE:
            HRAM[address - 0xFF80] = value;
            return;

        case 0xFFFF:
            IE = value;
            return;

        default:
            log_error("Invalid fetch address: %d", address);
            exit(1);       
    }
}