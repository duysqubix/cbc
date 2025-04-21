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

void dump_registers(){
    if (get_log_level() > LOG_TRACE){ return; }

    char buf[100];

    snprintf(buf, sizeof(buf), "\nA: %02X\t\tF: %04b\n" 
        "B: %02X\t\tC: %02X\n" 
        "D: %02X\t\tE: %02X\n" 
        "H: %02X\t\tL: %02X\t\t(HL): %02X\n" 
        "PC: %04X\tSP: %04X\n", 
        REG_A, REG_F>>4, REG_B, REG_C, REG_D, REG_E, REG_H, REG_L, _fetch_item(HL()), REG_PC, REG_SP);
    
    log_trace(buf);

}