#include <stdio.h>
#include <stdint.h>

int main() {
    uint16_t mmap[0xFFFF];
    uint8_t display[160*144*3];
    printf("%ld kilobytes\n", sizeof(mmap) / 1024);
    printf("%ld kilobytes\n", sizeof(display) / 1024);
    return 0;
} 