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



    // if(opcode == 0xCB){
    //     cb = true;
    //     INCR_PC();
    //     opcode = fetch_item(REG_PC);
    //     opcycles += 1;  // CB opcode takes 1 cycle by default.
    //     offset = 0x100;
    // }

    // size_t opcode_len_addr = (size_t)(opcode) + offset;
    // switch (OPCODE_LENGTH[opcode_len_addr]) {
    //     case 1:
    //         INCR_PC();
    //         break;

    //     // 8bit immediate
    //     case 2:
    //         value = fetch_item(REG_PC+1);
    //         INCR_PC();
    //         INCR_PC();
    //         break;

    //     // 16bit immediate
    //     case 3:
    //         value = (fetch_item(REG_PC+2) << 8) | fetch_item(REG_PC+1);
    //         INCR_PC();
    //         INCR_PC();
    //         INCR_PC();
    //         break;
    //     default:
    //         log_error("Invalid opcode length: %d for opcode: %02X. CB Mode: %s", OPCODE_LENGTH[(size_t)(opcode+offset)], opcode, cb ? "true" : "false");
    //         exit(1);
    // }


    // if (cb){
    //     log_trace("CB Opcode: CB:%02X[%d length] | %s", opcode, OPCODE_LENGTH[(size_t)(opcode+offset)], OPCODE_NAMES[(size_t)(opcode)+offset]);
    //     opcycles += _executeCB(opcode);
    // } else {
    //     log_trace("Opcode: %02X[%d length], Value: %04X | %s", opcode, OPCODE_LENGTH[(size_t)opcode], value, OPCODE_NAMES[(size_t)opcode]);

    //     opcycles += _execute(opcode, value);
    // }

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

inline opcycles_t _execute(opcode_t opcode, uint16_t value){
    bool prev_carry = false;

    // if(opcode >= 0x00 && opcode <= 0x0F){
    //     return opcodes[opcode](value);
    // }
    
    switch (opcode) {
        case 0x00: // NOP       
            return MCYCLE_1;

        case 0x01: // LD BC, nn 
            REG_B = U8(value >> 8);
            REG_C = U8(value & 0xFF);
            return MCYCLE_3;

        case 0x02: // LD (BC), A 
            WRITE_MEM(BC(), REG_A);
            return MCYCLE_2;

        case 0x03: // INC BC 
            INC_BC();
            return MCYCLE_1;

        case 0x04: // INC B
            REG_B++;
            return MCYCLE_1;

        case 0x05: // DEC B
            REG_B--;
            return MCYCLE_1;

        case 0x06: // LD B,n
            REG_B = value;
            return MCYCLE_2;

        case 0x07: // RLCA 
            FLAG_Z_RESET();
            FLAG_N_RESET();
            FLAG_H_RESET();
            FLAG_C_RESET();

            if BIT_IS_SET(REG_A, 7){
                FLAG_C_SET();
                REG_A = (REG_A << 1) + 1;
            } else {
                REG_A <<= 1;
            }

            return MCYCLE_1;

        case 0x08: // LD (nn), SP 
            WRITE_MEM(value, U8(REG_SP & 0xFF));
            WRITE_MEM(value + 1, U8(REG_SP >> 8));
            return MCYCLE_5;

        case 0x09: // ADD HL, BC
            ADD_SET_FLAGS16(HL(), BC());

            REG_H = U8((U16(HL()) + U16(BC())) >> 8);
            REG_L = U8((U16(HL()) + U16(BC())) & 0xFF);

            return MCYCLE_2;

        case 0x0A: // LD A, (BC)
            REG_A = READ_MEM(BC());
            return MCYCLE_2;

        case 0x0B: // DEC BC 
            DEC_BC();
            return MCYCLE_2;

        case 0x0C: // INC C
            REG_C++;
            return MCYCLE_1;

        case 0x0D: // DEC C
            REG_C--;
            return MCYCLE_1;

        case 0x0E: // LD C, n
            REG_C = value;
            return MCYCLE_2;

        case 0x0F: // RRCA
            FLAG_Z_RESET();
            FLAG_N_RESET();
            FLAG_H_RESET();
            FLAG_C_RESET();

            if BIT_IS_SET(REG_A, 0){
                FLAG_C_SET();
                REG_A = (REG_A >> 1) + 0x80;
            } else {
                REG_A >>= 1;
            }

            return MCYCLE_1;

        case 0x10: // STOP 
            CPU_HALTED = true;
            CPU_STUCK = true;
            return MCYCLE_1;

        case 0x11: // LD DE, nn
            REG_D = U8(value >> 8);
            REG_E = U8(value & 0xFF);
            return MCYCLE_3;

        case 0x12: // LD (DE), A
            WRITE_MEM(DE(), REG_A);
            return MCYCLE_2;
        
        case 0x13: // INC DE
            INC_DE();
            return MCYCLE_2;

        case 0x14: // INC D
            REG_D++;
            return MCYCLE_1;

        case 0x15: // DEC D
            REG_D--;
            return MCYCLE_1;

        case 0x16: // LD D, n
            REG_D = U8(value);
            return MCYCLE_2;

        case 0x17: // RLA
            FLAG_Z_RESET();
            FLAG_N_RESET();
            FLAG_H_RESET();

            prev_carry = FLAG_C_IS_SET();

            if BIT_IS_SET(REG_A, 7){
                FLAG_C_SET();
            }else{
                FLAG_C_RESET();
            }

            REG_A = (REG_A << 1) & 0xff;

            if (prev_carry){
                REG_A |= 0x01;
            }

            return MCYCLE_1;
            
            

        case 0x18: // JR r8
            REG_PC += i8(value);
            return MCYCLE_3;

        case 0x19: // ADD HL, DE 
            ADD_SET_FLAGS16(HL(), DE());

            REG_H = U8((U16(HL()) + U16(DE())) >> 8);
            REG_L = U8((U16(HL()) + U16(DE())) & 0xFF);

            return MCYCLE_2;

        case 0x1A: // LD A, (DE)
            REG_A = READ_MEM(DE());
            return MCYCLE_2;

        case 0x1B: // DEC DE
            DEC_DE();
            return MCYCLE_2;

        case 0x1C: // INC E
            REG_E++;
            return MCYCLE_1;

        case 0x1D: // DEC E
            REG_E--;
            return MCYCLE_1;

        case 0x1E: // LD E, d8 
            REG_E = U8(value);
            return MCYCLE_2;

        case 0x1F: // RRA
            FLAG_Z_RESET();
            FLAG_N_RESET();
            FLAG_H_RESET();

            prev_carry = FLAG_C_IS_SET();

            if BIT_IS_SET(REG_A, 0){
                FLAG_C_SET();
            }else{
                FLAG_C_RESET();
            }

            REG_A = (REG_A >> 1) & 0xFF;

            if (prev_carry){
                REG_A |= 0x80;
            }

            return MCYCLE_1;

        case 0x20: // JR NZ, e
            if (!FLAG_Z_IS_SET()){
                REG_PC += U8(value);
                return MCYCLE_3;
            }
            return MCYCLE_2;

        case 0x21: // LD HL, nn
            REG_H = U8(value >> 8);
            REG_L = U8(value & 0xFF);
            return MCYCLE_3;

        case 0x22: // LD (HL+), A
            WRITE_MEM(HL(), REG_A);
            INC_HL();
            return MCYCLE_2;

        case 0x23: // INC HL
            INC_HL();
            return MCYCLE_1;

        case 0x24: // INC H
            REG_H++;
            return MCYCLE_1;

        case 0x25: // DEC H
            REG_H--;
            return MCYCLE_1;

        case 0x26: // LD H, n
            REG_H = U8(value);
            return MCYCLE_2;

        case 0x27: // DAA
            uint16_t daa_result = U16(REG_A);

            FLAG_Z_RESET();

            if (FLAG_N_IS_SET()){
                if(FLAG_H_IS_SET()){
                    daa_result = (daa_result - 0x06) & 0xff;
                }

                if(FLAG_C_IS_SET()){
                    daa_result -= 0x60;
                }
            }else{
                if (FLAG_H_IS_SET() || (daa_result &0x0f) > 0x09){
                    daa_result += 0x06;
                }

                if (FLAG_C_IS_SET() || daa_result > 0x9F){
                    daa_result += 0x60;
                }
            }

            if((daa_result & 0xff) == 0 ){
                FLAG_Z_SET();
            }

            if((daa_result & 0x100) == 0x100){
                FLAG_C_SET();
            }

            FLAG_H_RESET();

            REG_A = (uint8_t)daa_result;

            return MCYCLE_2;
            

        case 0x28: // JR Z, e
            if (FLAG_Z_IS_SET()){
                REG_PC += U8(value);
                return MCYCLE_3;
            }
            return MCYCLE_2;

        case 0x2E: // LD L, d8
            REG_L = U8(value);
            return MCYCLE_2;

        case 0x37: // SCF
            FLAG_C_SET();
            return MCYCLE_1;

        case 0x30: // JR NC,e
            if (!FLAG_C_IS_SET()){  
                REG_PC += U8(value);
                return MCYCLE_3;
            }

            return MCYCLE_2;

        case 0x31: // LD SP, nn
            REG_SP = U16(value);
            return MCYCLE_2;

        case 0x36: // LD (HL), u8
            WRITE_MEM(HL(), U8(value));
            return MCYCLE_3;

        case 0x38: // JR C,e
            if (FLAG_C_IS_SET()){
                REG_PC += U8(value);
                return MCYCLE_3;
            }
            return MCYCLE_2;

        case 0x3C: // INC A
            REG_A++;
            return MCYCLE_1;

        case 0x3E: // LD A, n
            REG_A = U8(value);
            return MCYCLE_2;

        case 0x47: // LD B, A 
            REG_B = REG_A;
            return MCYCLE_1;

        case 0x4D: // LD C, L
            REG_C = REG_L;
            return MCYCLE_1;

        case 0x54: // LD D, H 
            REG_D = REG_H;
            return MCYCLE_1;

        case 0x55: // LD D, L
            REG_D = REG_L;
            return MCYCLE_1;

        case 0x57: // LD D, A
            REG_D = REG_A;
            return MCYCLE_1;

        case 0x5D: // LD E, L
            REG_E = REG_L;
            return MCYCLE_1;

        case 0x5F: // LD E, A
            REG_E = REG_A;
            return MCYCLE_1;

        case 0x62: // LD H, D 
            REG_H = REG_D;
            return MCYCLE_1;

        case 0x67: // LD H, A
            REG_H = REG_A;
            return MCYCLE_1;

        case 0x6B: // LD L, E
            REG_L = REG_E;
            return MCYCLE_1;

        case 0x6F: // LD L, A
            REG_L = REG_A;
            return MCYCLE_1;

        case 0x78: // LD A, B
            REG_A = REG_B;
            return MCYCLE_1;

        case 0x79: // LD A, C
            REG_A = REG_C;
            return MCYCLE_1;

        case 0x7A: // LD A, D 
            REG_A = REG_D;
            return MCYCLE_1;

        case 0x7B: // LD A, E
            REG_A = REG_E;
            return MCYCLE_1;

        case 0x7C: // LD A, H
            REG_A = REG_H;
            return MCYCLE_1;

        case 0x7D: // LD A, L
            REG_A = REG_L;
            return MCYCLE_1;

        case 0xAD: // XOR A, L
            XOR_SET_FLAGS(REG_A, REG_L);
            REG_A ^= REG_L;
            return MCYCLE_1;

        case 0xAF: // XOR A
            XOR_SET_FLAGS(REG_A, value);
            return MCYCLE_1;

        case 0xC1: // POP BC
            REG_B = READ_MEM(REG_SP + 1);
            REG_C = READ_MEM(REG_SP);
            REG_SP += 2;
            return MCYCLE_3;

        case 0xC2: // JP NZ, nn
            if (!FLAG_Z_IS_SET()){
                REG_PC = value;
                return MCYCLE_4;
            }
            return MCYCLE_3;

        case 0xC3: // JP nn
            REG_PC = value;
            return MCYCLE_4;

        case 0xC8: // RET Z
            if (FLAG_Z_IS_SET()){
                REG_PC = READ_MEM(REG_SP + 1) << 8 | READ_MEM(REG_SP);
                REG_SP += 2;
                return MCYCLE_4;
            }
            return MCYCLE_2;

        case 0xC9: // RET
            REG_PC = READ_MEM(REG_SP + 1) << 8 | READ_MEM(REG_SP);
            REG_SP += 2;
            return MCYCLE_4;

        case 0xCD: // CALL nn 
            WRITE_MEM(REG_SP - 1, REG_PC >> 8);
            WRITE_MEM(REG_SP - 2, REG_PC & 0xFF);
            REG_SP -= 2;
            REG_PC = value;
            return MCYCLE_6;

        case 0xE0: // LDH (a8), A
            WRITE_MEM(0xFF00 + value, REG_A);
            return MCYCLE_3;

        case 0xE1: // POP HL 
            REG_L = READ_MEM(REG_SP);
            REG_H = READ_MEM(REG_SP + 1);
            REG_SP += 2;
            return MCYCLE_3;

        case 0xE5: // PUSH HL 
            WRITE_MEM(REG_SP-1, REG_H);
            WRITE_MEM(REG_SP-2, REG_L);
            REG_SP -= 2;
            return MCYCLE_4;

        case 0xE6: // AND d8
            AND_SET_FLAGS(REG_A, value);
            REG_A &= (uint8_t)value;
            return MCYCLE_2;

        case 0xEA: // LD (a16), A
            WRITE_MEM(value, REG_A);
            return MCYCLE_4;
        
        case 0xF0: // LDH A, (a8)
            REG_A = READ_MEM((address_t)(0xFF00 + value));
            return MCYCLE_3;

        case 0xF1: // POP AF 
            REG_F = READ_MEM(REG_SP);
            REG_A = READ_MEM(REG_SP + 1);
            REG_SP += 2;
            return MCYCLE_3;

        case 0xF3: // DI
            IE = 0x00;
            return MCYCLE_1;

        case 0xF5: // PUSH AF 
            WRITE_MEM(REG_SP-1, REG_A);
            WRITE_MEM(REG_SP-2, REG_F);
            REG_SP -= 2;
            return MCYCLE_4;

        case 0xF6: // OR d8
            OR_SET_FLAGS(REG_A, value);
            REG_A |= (uint8_t)value;
            return MCYCLE_2;

        case 0xF8: // LD HL, SP+i8
            REG_H = U8((REG_SP + i8(value)) >> 8);
            REG_L = U8((REG_SP + i8(value)) & 0xFF);
            return MCYCLE_3;
        
        case 0xF9: // LD SP, HL
            REG_SP = (address_t)HL();
            return MCYCLE_2;

        case 0xFA: // LD A, (a16)
            REG_A = READ_MEM((address_t)value);
            return MCYCLE_4;

        case 0xFE: // CP d8
            CP_SET_FLAGS(REG_A, value);
            return MCYCLE_2;

        case 0xFF: // RST 38H
            WRITE_MEM(REG_SP - 1, REG_PC >> 8);
            WRITE_MEM(REG_SP - 2, REG_PC & 0xFF);
            REG_SP -= 2;
            REG_PC = 0x0038;
            return MCYCLE_4;

        default:
            log_error("Opcode: %02X[%d length], Value: %04X | %s", opcode, OPCODE_LENGTH[(size_t)opcode], value, OPCODE_NAMES[(size_t)opcode]);
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
            buf8 = fetch_item(HL());
            BIT_SET(buf8, 5);
            write_item(HL(), buf8);
            return MCYCLE_4;

        case 0xF6: // SET 6, (HL)
            buf8 = fetch_item(HL());
            BIT_SET(buf8, 6);
            write_item(HL(), buf8);
            return MCYCLE_4;

        default:
            log_error("CB Opcode: CB:%02X[%d length] | %s", opcode, OPCODE_LENGTH[(size_t)(opcode)], OPCODE_NAMES[(size_t)(opcode)]);
            exit(1);
    }
}