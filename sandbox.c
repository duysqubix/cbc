#include <stdio.h>
#include <stdint.h>
int main(){
    uint8_t a = 0xfa;

    printf("u8: %d, i8: %d\n", a, (int8_t)a);
    return 0;
}