/*
    This small CPU comparator is used to compare the register states before and after an opcode is executed 
    **WARNING** Only use opcodes that doesn't require reading/writing to memory as the Gb struct is only 
    partially initialized.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "src/gameboy.h"
#include "src/opcodes.h"

const char *bin_file = "/home/duys/.repos/GameboyCPUTests/main.bin";

static uint8_t gameboy_read(Gameboy *self, uint16_t address){
    return self->memory[address];
}

static void gameboy_write(Gameboy *self, uint16_t address, uint8_t value){
    self->memory[address] = value;
}


static void load_state(Gameboy *gb, uint8_t *buffer){
    gb->a = buffer[0];
    gb->b = buffer[1];
    gb->c = buffer[2];
    gb->d = buffer[3];
    gb->e = buffer[4];
    gb->f = buffer[5];
    gb->h = buffer[6];
    gb->l = buffer[7];

    gb->pc = buffer[8] << 8 | buffer[9];
    gb->sp = buffer[10] << 8 | buffer[11];
}

void dump_state(Gameboy *gb){
    char str[100];
    sprintf(str, "A: %02X B: %02X C: %02X D: %02X E: %02X F: %02X H: %02X L: %02X PC: %04X SP: %04X\n",
            gb->a, gb->b, gb->c, gb->d, gb->e, gb->f, gb->h, gb->l, gb->pc, gb->sp);
    printf("%s", str);
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


    for(int i = 0; i<size; i+=27){
        uint8_t opcode = buffer[i];
        uint8_t v1 = buffer[i+1];
        uint8_t v2 = buffer[i+2];

        printf("Testing opcode: %02X....", opcode);
        
        load_state(&actual_gb, buffer + i + 3);

        actual_gb.memory[actual_gb.pc] = opcode;
        actual_gb.memory[actual_gb.pc+1] = v1;
        actual_gb.memory[actual_gb.pc+2] = v2;


        load_state(&expected_gb, buffer + i + 12 + 3);
        // printf("Opcode: %02X Value: %04X\n", opcode, (uint16_t)(v1) << 8 | v2);
        opcode_def_t *op = opcodes[opcode];
        
        if(!op){
            printf("not implemented\n");
            continue;
        }

        op(&actual_gb);

        if(!compare_states(&actual_gb, &expected_gb)){
            printf("Opcode: %02X Value: %04X\n", opcode, (uint16_t)(v1) << 8 | v2);

            printf("States are not equal\n");
            dump_state(&actual_gb);
            dump_state(&expected_gb);
            exit(1);
            // break;
        }
        printf("done\n");
    }

    printf("Test passed\n");

    return 0;
}