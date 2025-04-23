#include <stdio.h>
#include "defs.h"
#include "log.h"

void fdump_memory(const char *filename, uint8_t *data, size_t size){
    FILE *file = fopen(filename, "w");
    if (!file) {
        log_error("Failed to open file: %s", filename);
        return;
    }

    for (size_t i = 0; i < size; i++) {
        if (i % 16 == 0) {
            fprintf(file, "\n$%04X: ", (uint16_t)i);
        }
        fprintf(file, "%02X ", data[i]);
    }
    fprintf(file, "\n");

    fclose(file);
}


void randomize(uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        data[i] = rand() % 256;
    }
}