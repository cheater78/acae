#include "stdlib.h"

int main(void) {
    printf("Hello from our Benchmark template!\n");
    printf("dec: %ld\n", 1234567890UL);
    printf("hex: %#08lx\n", 0xF0A9);
    printf("float: %06.3f\n", -123.321f); // front padding not working
    return 0;
}