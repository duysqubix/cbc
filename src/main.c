#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "log.h"
#include "gameboy.h"

void logging_init(){
    const char *log_level = getenv("LOG_LEVEL");
    log_set_level(LOG_WARN);
    if (log_level){
        if (strcmp(log_level, "trace") == 0) {
            log_set_level(LOG_TRACE);
        } else if (strcmp(log_level, "debug") == 0) {
            log_set_level(LOG_DEBUG);
        } else if (strcmp(log_level, "info") == 0) {
            log_set_level(LOG_INFO);
        } else {
            fprintf(stderr, "Invalid log level: %s", log_level);
            exit(1);
        }
    }
    log_warn("Log level: %s", log_level);
}

int main(int argc, char *argv[]){
    if (argc != 2){
        log_error("Usage: %s <rom_file>", argv[0]);
        return 1;
    }

    logging_init();

    Gameboy *gb = gameboy_new(argv[1]);
    if (!gb){
        log_error("Failed to create Gameboy");
        return 1;
    }

    GameboyState state = gameboy_run_until_complete(gb);
    if (state == GAMEBOY_ERROR){
        log_error("Error: Gameboy state is error");
        gb->free(gb);
        return 1;
    }
    gb->free(gb);
    return 0;
}