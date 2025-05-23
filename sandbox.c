/*
    This small CPU comparator is used to compare the register states before and after an opcode is executed 
    **WARNING** Only use opcodes that doesn't require reading/writing to memory as the Gb struct is only 
    partially initialized.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "src/gameboy.h"
#include "src/opcodes.h"

const char *bin_file = "main.bin";
static char * const OPCODE_NAMES[512];

static uint8_t gameboy_read(Gameboy *self, uint16_t address){
    // printf("reading [%02X] from %04X\n", self->memory[address], address);
    return self->memory[address];
}

static void gameboy_write(Gameboy *self, uint16_t address, uint8_t value){
    // printf("writing to %04X: %02X\n", address, value);
    self->memory[address] = value;
}

static void clear_memory(Gameboy *gb){
    for(int i = 0; i < 0x10000; i++){
        gb->memory[i] = 0;
    }
}



static void load_state(Gameboy *gb, uint8_t *buffer){
    uint16_t addr_stack[6];
    uint8_t val_stack[6];

    clear_memory(gb);
    gb->a = buffer[0];
    gb->b = buffer[1];
    gb->c = buffer[2];
    gb->d = buffer[3];
    gb->e = buffer[4];
    gb->f = buffer[5];
    gb->h = buffer[6];
    gb->l = buffer[7];

    gb->pc = (buffer[8] << 8 | buffer[9])-1;
    gb->sp = buffer[10] << 8 | buffer[11];

    int valid_addr_counter = 0;

    uint16_t addr1 = buffer[12] << 8 | buffer[13];
    uint8_t val1 = buffer[14];
    if(val1 != 0) {
        addr_stack[valid_addr_counter] = addr1;
        val_stack[valid_addr_counter] = val1;
        valid_addr_counter++;
    }

    uint16_t addr2 = buffer[15] << 8 | buffer[16];
    uint8_t val2 = buffer[17];
    if(val2 != 0) {
        addr_stack[valid_addr_counter] = addr2;
        val_stack[valid_addr_counter] = val2;
        valid_addr_counter++;
    }

    uint16_t addr3 = buffer[18] << 8 | buffer[19];
    uint8_t val3 = buffer[20];
    if(val3 != 0) {
        addr_stack[valid_addr_counter] = addr3;
        val_stack[valid_addr_counter] = val3;
        valid_addr_counter++;
    }

    uint16_t addr4 = buffer[21] << 8 | buffer[22];
    uint8_t val4 = buffer[23];
    if(val4 != 0) {
        addr_stack[valid_addr_counter] = addr4;
        val_stack[valid_addr_counter] = val4;
        valid_addr_counter++;
    }

    uint16_t addr5 = buffer[24] << 8 | buffer[25];
    uint8_t val5 = buffer[26];
    if(val5 != 0) {
        addr_stack[valid_addr_counter] = addr5;
        val_stack[valid_addr_counter] = val5;
        valid_addr_counter++;
    }

    uint16_t addr6 = buffer[27] << 8 | buffer[28];
    uint8_t val6 = buffer[29];
    if(val6 != 0) {
        addr_stack[valid_addr_counter] = addr6;
        val_stack[valid_addr_counter] = val6;
        valid_addr_counter++;
    }

    for(int i = 0; i < valid_addr_counter; i++){
        gb->memory[addr_stack[i]] = val_stack[i];
    }

}

void dump_state(Gameboy *gb){
    printf("A: %02X B: %02X C: %02X D: %02X E: %02X F: %02X H: %02X L: %02X PC: %04X SP: %04X\n",
           gb->a, gb->b, gb->c, gb->d, gb->e, (gb->f >> 4), gb->h, gb->l, gb->pc, gb->sp);
    for(int i = 0; i < 0x10000; i++){
        if(gb->memory[i] != 0){
            printf("Memory: [%04X] = %02X\n", i, gb->memory[i]);
        }
    }
}

int compare_states(Gameboy *gb1, Gameboy *gb2){
    if(gb1->a != gb2->a){
        return 0;
    }
    if(gb1->b != gb2->b){
        return 0;
    }
    if(gb1->c != gb2->c){
        return 0;
    }
    if(gb1->d != gb2->d){
        return 0;
    }
    if(gb1->e != gb2->e){
        return 0;
    }
    if(gb1->f != gb2->f){
        return 0;
    }
    if(gb1->h != gb2->h){
        return 0;
    }
    if(gb1->l != gb2->l){
        return 0;
    }
    if(gb1->pc != gb2->pc){
        return 0;
    }
    if(gb1->sp != gb2->sp){
        return 0;
    }
    for(int i = 0; i < 0x10000; i++){
        if(gb1->memory[i] != gb2->memory[i]){
            printf("Memory mismatch at %04X: %02X != %02X\n", i, gb1->memory[i], gb2->memory[i]);
            return 0;
        }
    }

    return 1;
}

int main(){

    FILE *bin = fopen(bin_file, "rb");
    
    if(!bin){
        fprintf(stderr, "Failed to open file: %s\n", bin_file);
        return 1;
    }
    
    fseek(bin, 0, SEEK_END);
    int size = ftell(bin);
    fseek(bin, 0, SEEK_SET);

    uint8_t buffer[size];
    fread(buffer, 1, size, bin);
    fclose(bin);

    Gameboy actual_gb;
    actual_gb.read = gameboy_read;
    actual_gb.write = gameboy_write;

    Gameboy expected_gb;

    uint8_t current_opcode = 0xFF;
    for(int i = 0; i<size; i+=0x3f){
        uint16_t opcode = buffer[i];
        uint16_t offset = 0;
        uint8_t v1 = buffer[i+1];
        uint8_t v2 = buffer[i+2];

        if(opcode == 0xCB){
            opcode = buffer[i+1];
            offset = 0x100;
            continue;
        }

        if(opcode != current_opcode){
            printf("Testing opcode: %s%02X [%s]....................", offset ? "CB " : "", opcode, OPCODE_NAMES[opcode+offset]);
        }
        load_state(&actual_gb, buffer + i + 3);
        load_state(&expected_gb, buffer + i +0x21);


        opcode_def_t *op = opcodes[opcode + offset];
        
        if(!op){
            if (opcode != current_opcode){
                printf("not implemented\n");
                current_opcode = opcode;
            }
            exit(1);
        }

        // dump_state(&actual_gb);
        op(&actual_gb);
        // dump_state(&actual_gb);

        if(!compare_states(&actual_gb, &expected_gb)){
            printf("Opcode: %02X Value: %04X\n", opcode, (uint16_t)(v1) << 8 | v2);

            printf("States are not equal\n");
            dump_state(&actual_gb);
            dump_state(&expected_gb);
            exit(1);
            // break;
        }
        if (opcode != current_opcode){
            printf("passed\n");
            current_opcode = opcode;
        }
    }

    printf("Test passed\n");

    return 0;
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