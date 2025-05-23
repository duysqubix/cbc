#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "gameboy.h"
#include "log.h"
#include "opcodes.h"
static void gameboy_free(Gameboy *self);
static uint8_t gameboy_read(Gameboy *self, uint16_t address);
static void gameboy_write(Gameboy *self, uint16_t address, uint8_t value);
static gbcycles_t gameboy_tick(Gameboy *self);
static void dump_registers(Gameboy *gb, uint8_t opcode);

static char * const OPCODE_NAMES[512];
static uint8_t const OPCODE_LENGTH[512];

static FILE *log_file = NULL;

Gameboy *gameboy_new(const char *rom_filename){

    if (gflags.trace_file){
        gflags.trace_state = 1;
        log_file = fopen(gflags.trace_file, "w");
        log_add_fp(log_file, get_log_level());
    }
    
    Gameboy *gb = (Gameboy *)malloc(sizeof(Gameboy));
    if (!gb){
        return NULL;
    }

    Cartridge *cart = new_cartridge();
    
    if(!cart->load_rom(cart, rom_filename)){
        log_error("Failed to load ROM: %s", rom_filename);
        gameboy_free(gb);
        return NULL;
    }

    gb->cartridge = cart;

    if (!gb->cartridge){
        gameboy_free(gb);
        return NULL;
    }


    // assign public methods 
    gb->free = gameboy_free;
    gb->read = gameboy_read;
    gb->write = gameboy_write;
    gb->tick = gameboy_tick;

    // set registers to initial values 

    gb->a = 0x01;
    gb->f = 0xb0;
    gb->b = 0x00;
    gb->c = 0x13;
    gb->d = 0x00;
    gb->e = 0xd8;
    gb->h = 0x01;
    gb->l = 0x4d;
    gb->pc = 0x0100;
    gb->sp = 0xfffe;

    // randomize memory
    for(int i=_SRAM; i<_SRAM+0x1FFF; i++){
        gb->memory[i] = rand() % 256;
    }

    return gb;
}

static void gameboy_free(Gameboy *self){
    if (self->cartridge){
        log_trace("Freeing Cartridge");
        self->cartridge->free(self->cartridge);
    }

    if (self){
        log_trace("Freeing Gameboy");
        free(self);
    }

    if (log_file){
        fclose(log_file);
    }
}



static gbcycles_t gameboy_tick(Gameboy *self){
    gbcycles_t  cycles = 0;
    size_t      offset = 0;
    uint8_t opcode = self->read(self, self->pc);

    uint16_t prev_pc = self->pc;
    uint16_t prev_sp = self->sp;

    if (opcode == 0xCB){
        self->pc++;
        opcode = self->read(self, self->pc);
        offset = 0x100;
    }
    opcode_def_t *op = opcodes[opcode + offset];
    if (!op){
        log_error("Invalid opcode: %02X", opcode);
        return 0xFFFFFFFF;
    }
    
    if ((gflags.step_at.address == self->pc || gflags.step_at.enabled) && get_log_level() <= LOG_TRACE){
        gflags.trace_state = 0;
        gflags.step_at.enabled = true;
        dump_registers(self, opcode+offset);
        getchar();
    }

    if (gflags.trace_state){
        dump_registers(self, opcode+offset);
    }

    cycles += op(self);


    if (!self->halted && (prev_pc == self->pc) && (prev_sp == self->sp) && !self->is_stuck){
        log_error("Gameboy is stuck at PC: %04X, SP: %04X", self->pc, self->sp);
        self->is_stuck = 1;
    }


    // tick the cpu 
    // tick the cartridge 
    // tick the timer 
    // tick the lcd 
    // handle interrupts
    // getchar();
    return cycles;
}


static uint8_t gameboy_read(Gameboy *self, uint16_t address){
    Cartridge *cart = self->cartridge;

    switch(address){
        case 0x0000 ... 0x7FFF:
            return cart->read(cart, address);
        case 0xA000 ... 0xBFFF:
            return cart->read(cart, address);

        case 0xE000 ... 0xFDFF:
            return self->memory[address - 0xE000];
        
        case 0xFFFF:
            return self->ie;

        default:
            if (address == _IO_LY){
                return 0x90;
            }
            return self->memory[address];
    }
}

static void gameboy_write(Gameboy *self, uint16_t address, uint8_t value){
    Cartridge *cart = self->cartridge;

    if (address == _IO_SC && value == 0x81){
        printf("%c", self->read(self, _IO_SB));
    }

    switch(address){
        case 0x0000 ... 0x7FFF:
            cart->write(cart, address, value);
            break;
        case 0xA000 ... 0xBFFF:
            cart->write(cart, address, value);
            break;
        case 0xE000 ... 0xFDFF:
            self->memory[address - 0xE000] = value;
            break;
        case 0xFFFF:
            self->ie = value;
            break;
        default:
            self->memory[address] = value;
            break;
    }
}

static void dump_registers(Gameboy *gb, uint8_t opcode){
    const char *str = 
    "ROM%02X:%04X\t%-20s BC:%04X DE:%04X HL:%04X AF:%04X SP:%04X PC:%04X"
    "| %02X %02X [%02X] %02X %02X | ZNHC:%04b" ;

    uint16_t af      = (uint16_t)(gb->a << 8) | gb->f;
    uint16_t bc      = (uint16_t)(gb->b << 8) | gb->c;
    uint16_t de      = (uint16_t)(gb->d << 8) | gb->e;
    uint16_t hl      = (uint16_t)(gb->h << 8) | gb->l;
    uint8_t bank     = gb->cartridge->rom_bank_select;
    
    if (gb->pc < 0x4000){
        bank = 0;
    }

    log_trace(str, 
        bank,
        gb->pc,
        OPCODE_NAMES[opcode],
        bc, de, hl, af, gb->sp, gb->pc,
        gb->read(gb, gb->pc-2), gb->read(gb, gb->pc-1), opcode, gb->read(gb, gb->pc+1), gb->read(gb, gb->pc+2),
        gb->f>>4
    );

}


static char * const OPCODE_NAMES[512] = {
	"NOP", "LD BC, d16", "LD (BC), A", "INC BC", "INC B", "DEC B", "LD B, d8", "RLCA", "LD (a16), SP", "ADD HL, BC", "LD A, (BC)", "DEC BC", "INC C", "DEC C", "LD C, d8", "RRCA",
	"STOP 0", "LD DE, d16", "LD (DE), A", "INC DE", "INC D", "DEC D", "LD D, d8", "RLA", "JR r8", "ADD HL, DE", "LD A, (DE)", "DEC DE", "INC E", "DEC E", "LD E, d8", "RRA",
	"JR NZ, r8", "LD HL, d16", "LD (HL+), A", "INC HL", "INC H", "DEC H", "LD H, d8", "DAA", "JR Z, r8", "ADD HL, HL", "LD A, (HL+)", "DEC HL", "INC L", "DEC L", "LD L, d8", "CPL",
	"JR NC, r8", "LD SP, d16", "LD (HL-), A", "INC SP", "INC (HL)", "DEC (HL)", "LD (HL), d8", "SCF", "JR C, r8", "ADD HL, SP", "LD A, (HL-)", "DEC SP", "INC A", "DEC A", "LD A, d8", "CCF",
	"LD B, B", "LD B, C", "LD B, D", "LD B, E", "LD B, H", "LD B, L", "LD B, (HL)", "LD B, A", "LD C, B", "LD C, C", "LD C, D", "LD C, E", "LD C, H", "LD C, L", "LD C, (HL)", "LD C, A",
	"LD D, B", "LD D, C", "LD D, D", "LD D, E", "LD D, H", "LD D, L", "LD D, (HL)", "LD D, A", "LD E, B", "LD E, C", "LD E, D", "LD E, E", "LD E, H", "LD E, L", "LD E, (HL)", "LD E, A",
	"LD H, B", "LD H, C", "LD H, D", "LD H, E", "LD H, H", "LD H, L", "LD H, (HL)", "LD H, A", "LD L, B", "LD L, C", "LD L, D", "LD L, E", "LD L, H", "LD L, L", "LD L, (HL)", "LD L, A",
	"LD (HL), B", "LD (HL), C", "LD (HL), D", "LD (HL), E", "LD (HL), H", "LD (HL), L", "HALT", "LD (HL), A", "LD A, B", "LD A, C", "LD A, D", "LD A, E", "LD A, H", "LD A, L", "LD A, (HL)", "LD A, A",
	"ADD A, B", "ADD A, C", "ADD A, D", "ADD A, E", "ADD A, H", "ADD A, L", "ADD A, (HL)", "ADD A, A", "ADC A, B", "ADC A, C", "ADC A, D", "ADC A, E", "ADC A, H", "ADC A, L", "ADC A, (HL)", "ADC A, A",
	"SUB B", "SUB C", "SUB D", "SUB E", "SUB H", "SUB L", "SUB (HL)", "SUB A", "SBC A, B", "SBC A, C", "SBC A, D", "SBC A, E", "SBC A, H", "SBC A, L", "SBC A, (HL)", "SBC A, A",
	"AND B", "AND C", "AND D", "AND E", "AND H", "AND L", "AND (HL)", "AND A", "XOR B", "XOR C", "XOR D", "XOR E", "XOR H", "XOR L", "XOR (HL)", "XOR A",
	"OR B", "OR C", "OR D", "OR E", "OR H", "OR L", "OR (HL)", "OR A", "CP B", "CP C", "CP D", "CP E", "CP H", "CP L", "CP (HL)", "CP A",
	"RET NZ", "POP BC", "JP NZ, a16", "JP a16", "CALL NZ, a16", "PUSH BC", "ADD A, d8", "RST 00H", "RET Z", "RET", "JP Z, a16", "PREFIX CB", "CALL Z, a16", "CALL a16", "ADC A, d8", "RST 08H",
	"RET NC", "POP DE", "JP NC, a16", "ILLEGAL", "CALL NC, a16", "PUSH DE", "SUB d8", "RST 10H", "RET C", "RETI", "JP C, a16", "ILLEGAL", "CALL C, a16", "ILLEGAL", "SBC A, d8", "RST 18H",
	"LDH (a8), A", "POP HL", "LD (C), A", "ILLEGAL", "ILLEGAL", "PUSH HL", "AND d8", "RST 20H", "ADD SP, r8", "JP (HL)", "LD (a16), A", "ILLEGAL", "ILLEGAL", "ILLEGAL", "XOR d8", "RST 28H",
	"LDH A, (a8)", "POP AF", "LD A, (C)", "DI", "ILLEGAL", "PUSH AF", "OR d8", "RST 30H", "LD HL, SP+r8", "LD SP, HL", "LD A, (a16)", "EI", "ILLEGAL", "ILLEGAL", "CP d8", "RST 38H",
	// CB prefix instructions do not take any arguments
	"RLC B", "RLC C", "RLC D", "RLC E", "RLC H", "RLC L", "RLC (HL)", "RLC A", "RRC B", "RRC C", "RRC D", "RRC E", "RRC H", "RRC L", "RRC (HL)", "RRC A",
	"RL B", "RL C", "RL D", "RL E", "RL H", "RL L", "RL (HL)", "RL A", "RR B", "RR C", "RR D", "RR E", "RR H", "RR L", "RR (HL)", "RR A",
	"SLA B", "SLA C", "SLA D", "SLA E", "SLA H", "SLA L", "SLA (HL)", "SLA A", "SRA B", "SRA C", "SRA D", "SRA E", "SRA H", "SRA L", "SRA (HL)", "SRA A",
	"SWAP B", "SWAP C", "SWAP D", "SWAP E", "SWAP H", "SWAP L", "SWAP (HL)", "SWAP A", "SRL B", "SRL C", "SRL D", "SRL E", "SRL H", "SRL L", "SRL (HL)", "SRL A",
	"BIT 0, B", "BIT 0, C", "BIT 0, D", "BIT 0, E", "BIT 0, H", "BIT 0, L", "BIT 0, (HL)", "BIT 0, A", "BIT 1, B", "BIT 1, C", "BIT 1, D", "BIT 1, E", "BIT 1, H", "BIT 1, L", "BIT 1, (HL)", "BIT 1, A",
	"BIT 2, B", "BIT 2, C", "BIT 2, D", "BIT 2, E", "BIT 2, H", "BIT 2, L", "BIT 2, (HL)", "BIT 2, A", "BIT 3, B", "BIT 3, C", "BIT 3, D", "BIT 3, E", "BIT 3, H", "BIT 3, L", "BIT 3, (HL)", "BIT 3, A",
	"BIT 4, B", "BIT 4, C", "BIT 4, D", "BIT 4, E", "BIT 4, H", "BIT 4, L", "BIT 4, (HL)", "BIT 4, A", "BIT 5, B", "BIT 5, C", "BIT 5, D", "BIT 5, E", "BIT 5, H", "BIT 5, L", "BIT 5, (HL)", "BIT 5, A",
	"BIT 6, B", "BIT 6, C", "BIT 6, D", "BIT 6, E", "BIT 6, H", "BIT 6, L", "BIT 6, (HL)", "BIT 6, A", "BIT 7, B", "BIT 7, C", "BIT 7, D", "BIT 7, E", "BIT 7, H", "BIT 7, L", "BIT 7, (HL)", "BIT 7, A",
	"RES 0, B", "RES 0, C", "RES 0, D", "RES 0, E", "RES 0, H", "RES 0, L", "RES 0, (HL)", "RES 0, A", "RES 1, B", "RES 1, C", "RES 1, D", "RES 1, E", "RES 1, H", "RES 1, L", "RES 1, (HL)", "RES 1, A",
	"RES 2, B", "RES 2, C", "RES 2, D", "RES 2, E", "RES 2, H", "RES 2, L", "RES 2, (HL)", "RES 2, A", "RES 3, B", "RES 3, C", "RES 3, D", "RES 3, E", "RES 3, H", "RES 3, L", "RES 3, (HL)", "RES 3, A",
	"RES 4, B", "RES 4, C", "RES 4, D", "RES 4, E", "RES 4, H", "RES 4, L", "RES 4, (HL)", "RES 4, A", "RES 5, B", "RES 5, C", "RES 5, D", "RES 5, E", "RES 5, H", "RES 5, L", "RES 5, (HL)", "RES 5, A",
	"RES 6, B", "RES 6, C", "RES 6, D", "RES 6, E", "RES 6, H", "RES 6, L", "RES 6, (HL)", "RES 6, A", "RES 7, B", "RES 7, C", "RES 7, D", "RES 7, E", "RES 7, H", "RES 7, L", "RES 7, (HL)", "RES 7, A",
	"SET 0, B", "SET 0, C", "SET 0, D", "SET 0, E", "SET 0, H", "SET 0, L", "SET 0, (HL)", "SET 0, A", "SET 1, B", "SET 1, C", "SET 1, D", "SET 1, E", "SET 1, H", "SET 1, L", "SET 1, (HL)", "SET 1, A",
	"SET 2, B", "SET 2, C", "SET 2, D", "SET 2, E", "SET 2, H", "SET 2, L", "SET 2, (HL)", "SET 2, A", "SET 3, B", "SET 3, C", "SET 3, D", "SET 3, E", "SET 3, H", "SET 3, L", "SET 3, (HL)", "SET 3, A",
	"SET 4, B", "SET 4, C", "SET 4, D", "SET 4, E", "SET 4, H", "SET 4, L", "SET 4, (HL)", "SET 4, A", "SET 5, B", "SET 5, C", "SET 5, D", "SET 5, E", "SET 5, H", "SET 5, L", "SET 5, (HL)", "SET 5, A",
	"SET 6, B", "SET 6, C", "SET 6, D", "SET 6, E", "SET 6, H", "SET 6, L", "SET 6, (HL)", "SET 6, A", "SET 7, B", "SET 7, C", "SET 7, D", "SET 7, E", "SET 7, H", "SET 7, L", "SET 7, (HL)", "SET 7, A",
};

static uint8_t const OPCODE_LENGTH[512] = {
	1, 3, 1, 1, 1, 1, 2, 1, 3, 1, 1, 1, 1, 1, 2, 1, // 0x00-0x0F
	2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, // 0x10-0x1F
	2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, // 0x20-0x2F
	2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, // 0x30-0x3F
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x40-0x4F
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x50-0x5F
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x60-0x6F
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x70-0x7F
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x80-0x8F
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x90-0x9F
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xA0-0xAF
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xB0-0xBF
	1, 1, 3, 3, 3, 1, 2, 1, 1, 1, 3, 1, 3, 3, 2, 1, // 0xC0-0xCF
	1, 1, 3, 0, 3, 1, 2, 1, 1, 1, 3, 0, 3, 0, 2, 1, // 0xD0-0xDF
	2, 1, 1, 0, 0, 1, 2, 1, 2, 1, 3, 0, 0, 0, 2, 1, // 0xE0-0xEF
	2, 1, 1, 1, 0, 1, 2, 1, 2, 1, 3, 1, 0, 0, 2, 1, // 0xF0-0xFF
	// CB prefix instructions do not take any arguments
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xCB00-0xCB0F
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xCB10-0xCB1F
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xCB20-0xCB2F
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xCB30-0xCB3F
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xCB40-0xCB4F
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xCB50-0xCB5F
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xCB60-0xCB6F
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xCB70-0xCB7F
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xCB80-0xCB8F
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xCB90-0xCB9F
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xCBA0-0xCBAF
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xCBB0-0xCBBF
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xCBC0-0xCBCF
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xCBD0-0xCBDF
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xCBE0-0xCBEF
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1  // 0xCBF0-0xCBFF
};