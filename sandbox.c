#include <stdio.h>
#include <stdint.h>
int main(){
    uint8_t a = 0x00;

    for(int i =0; i< 256; i++){
        printf("i: %02X\t&i: %02X\n", i, i&0xf);
    }
    return 0;
}