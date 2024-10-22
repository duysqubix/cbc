#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include "gameboy.h"
#include "opcodes.h"


// 25 mb buffer
int BUFFER_SIZE = 1024 * 1024 * 25;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    GAMEBOY gb;

    // get filename
    char *filename = argv[1];
    gameboy_init(&gb, filename);
    gameboy_status(&gb);
    execute_opcode(&gb, 0x01, NULL);
    gameboy_status(&gb);

    gameboy_free(&gb);

    return 0;
}