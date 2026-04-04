#include "dwt.h"
#include "printf.h"
#include "stdlib.h"
#include "sys.h"

int main(void) {
    printf("Hello from our Benchmark template!\n");
    printf("dec: %ld\n", 1234567890UL);
    printf("hex: %#08lx\n", 0xF0A9);
    printf("float: %06.3f\n", -123.321f); // front padding not working

    volatile unsigned int ctrl = 1;
    //ctrl = DWT_CTRL;
    printf("DWT_CTRL:%x", ctrl);
    printf("DWT_CYCCNT:%x", DWT_CYCCNT);

    for(unsigned int i = 0; i < (1<<3); i++) {
        volatile unsigned int cycs = dwt_cyccnt();
        printf("[%u]:%u\n", i, cycs);
    }

    return 0;
}