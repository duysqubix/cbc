#ifndef OPCODES_H__
#define OPCODES_H__


#include <stdint.h>
#include "gameboy.h"

// macro to convert uint16_t to uint8_t based on little endian
#define HIGH_BYTE(word) ((uint8_t)((word >> 8) & 0xFF))
#define LOW_BYTE(word) ((uint8_t)(word & 0xFF))

typedef uint64_t opcycle_t;
typedef uint8_t opcode_t;
typedef uint8_t byte_t;
typedef uint16_t word_t;
typedef opcycle_t (*opcode_func)(GAMEBOY *gb, void *value);

extern opcycle_t execute_opcode(GAMEBOY *gb, uint8_t opcode, void *opcode_value);


#define OPCYCLE 4

#endif