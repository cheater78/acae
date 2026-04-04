#include "dwt.h"

void dwt_init() {
    //SCB_DEMCR = SCB_DEMCR | 0x01000000;
    DWT_LAR = 0xC5ACCE55;
    DWT_CYCCNT = 0;
    DWT_CTRL = DWT_CTRL | 1;
}

unsigned int dwt_cyccnt() {
    return DWT_CYCCNT;
}