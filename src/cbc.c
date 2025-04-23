#include <stdio.h>
#include <time.h>
#include "defs.h"
#include "log.h"



opcycles_t  _tick();
opcycles_t  _tick_cpu(opcycles_t cycles);

uint8_t     _fetch_item(uint16_t address);
void        _write_item(uint16_t address, uint16_t value);
opcycles_t  _execute(opcode_t opcode, uint16_t value);
opcycles_t _executeCB(opcode_t opcode);

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
    static opcycles_t cycles = 4; 

    dump_registers();

    //update cpu 
    cycles = _tick_cpu(cycles);

    //update cartridge
    //update timers
    //update lcd
    // handle interrupts
    if (STEP_MODE) {
        getchar();
    }
    return 0;
}

opcycles_t _tick_cpu(opcycles_t cycles){
    opcycles_t opcycles = 0;
    opcode_t opcode = (opcode_t)_fetch_item(REG_PC);
    uint16_t value = 0;

    size_t offset = 0;
    bool cb = false;

    if(opcode == 0xCB){
        cb = true;
        INCR_PC();
        opcode = _fetch_item(REG_PC);
        opcycles += 1;  // CB opcode takes 1 cycle by default.
        offset = 0x100;
    }

    log_trace("Opcode Length: %d", OPCODE_LENGTH[(size_t)(opcode+offset)]);
    switch (OPCODE_LENGTH[(size_t)(opcode+offset)]) {
        case 1:
            INCR_PC();
            break;

        // 8bit immediate
        case 2:
            value = _fetch_item(REG_PC+1);
            INCR_PC();
            INCR_PC();
            break;

        // 16bit immediate
        case 3:
            value = (_fetch_item(REG_PC+2) << 8) | _fetch_item(REG_PC+1);
            INCR_PC();
            INCR_PC();
            INCR_PC();
            break;
    }


    if (cb){
        log_trace("CB Opcode: CB:%02X | %s", opcode, OPCODE_NAMES[(size_t)(opcode)+offset]);
        opcycles += _executeCB(opcode);
    } else {
        log_trace("|Opcode: %02X, Value: %04X | %s", opcode, value, OPCODE_NAMES[(size_t)opcode]);

        opcycles += _execute(opcode, value);
    }

    return opcycles+cycles;
}

uint8_t _fetch_item(uint16_t address){
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

void _write_item(uint16_t address, uint16_t value){
    ROM[address] = value;
}

inline opcycles_t _execute(opcode_t opcode, uint16_t value){
    uint16_t sp_high = 0x00;
    uint16_t sp_low = 0x00;
    uint8_t buf8 = 0x00;
    

    switch (opcode) {
        case 0x00: // NOP       
            return MCYCLE_1;

        case 0x01: // LD BC, nn 
            REG_B = value & 0xFF;
            REG_C = value >> 8;
            return MCYCLE_3;

        case 0x06: // LD B,n
            REG_B = value;
            return MCYCLE_2;

        case 0x21:
            REG_H = value >> 8;
            REG_L = value & 0xFF;
            return MCYCLE_3;

        case 0x37: // SCF
            FLAG_C_SET();
            return MCYCLE_1;

        case 0x30: // JR NC,e
            buf8 = (uint8_t)(value);
            if (!FLAG_C_IS_SET()){  
                REG_PC += buf8;
                return MCYCLE_3;
            }

            REG_PC += 2;
            return MCYCLE_2;

        case 0x31: // LD SP, nn
            REG_SP = HL();
            return MCYCLE_2;

        case 0x38: // JR C,e
            buf8 = (uint8_t)(value);
            if (FLAG_C_IS_SET()){
                REG_PC += buf8;
                return MCYCLE_3;
            }

            REG_PC += 2;
            return MCYCLE_2;
            

        case 0xC3: // JP nn
            REG_PC = value;
            return MCYCLE_4;
        
        case 0xF3: // DI
            IE = 0x00;
            return MCYCLE_1;

        case 0xF6: // OR d8
            FLAG_N_RESET();
            FLAG_H_RESET();
            FLAG_C_RESET();
            FLAG_Z_RESET();

            REG_A |= (uint8_t)value;

            if (!REG_A){ FLAG_Z_SET();}

            return MCYCLE_2;

        case 0xFF: // RST 38H
            sp_high = REG_SP -1;
            sp_low = REG_SP -2;
            _write_item(sp_high, REG_PC >> 8);
            _write_item(sp_low, REG_PC & 0xFF);

            REG_SP -= 2;
            REG_PC = 0x0038;
            return MCYCLE_4;

        default:
            log_error("Invalid opcode: %02X", opcode);
            exit(1);
    }
}

opcycles_t _executeCB(opcode_t opcode){
    // uint16_t buf16 = 0x00;
    uint8_t buf8 = 0x00;
    switch (opcode) {
        case 0x00: // NOP
            return MCYCLE_1;

        case 0xEE: // SET 5, (HL)
            buf8 = _fetch_item(HL());
            BIT_SET(buf8, 5);
            _write_item(HL(), buf8);
            return MCYCLE_4;

        case 0xF6: // SET 6, (HL)
            buf8 = _fetch_item(HL());
            BIT_SET(buf8, 6);
            _write_item(HL(), buf8);
            return MCYCLE_4;

        default:
            log_error("Invalid CB opcode: %02X", opcode);
            exit(1);
    }
}