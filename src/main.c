#include <stdio.h>
#include "defs.h"
#include "log.h"

struct {
    int dump;
} flags;


int main(int argc, char *argv[]) {
    log_set_level(LOG_TRACE);

    gameboy_init(argv[1]);

    return gameboy_loop();
    // printf("AF: %04X\n", AF());
    return 0;
} 