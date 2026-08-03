#include "dwt.h"

#include "printf.h"

void init_dwt() {
    // HW needs debug enabled first! -> platform_init
    DWT_LAR = 0xC5ACCE55; // unlock DWT (not needed for qemu, but required for real hardware)
    DWT_CYCCNT = 0;
    DWT_CTRL |= (1 << 0);
    
    printf("DWT_LAR = %08lx\n", DWT_LAR);
    printf("DWT_CTRL = %08lx\n", DWT_CTRL);
    printf("DWT_CYCCNT = %08lx\n", DWT_CYCCNT);
}