#pragma once

// platform environment
#include <platform.h>

#define _DWT_ADDR_SPACE_SIZE 0x1000

#define DWT_ADDR_CTRL       (_DWT_BASE_MEM_ADDR + 0x000)
#define DWT_ADDR_CYCCNT     (_DWT_BASE_MEM_ADDR + 0x004)
#define DWT_ADDR_LAR        (_DWT_BASE_MEM_ADDR + 0xFB0)

// dwt registers
#define DWT_CTRL    *(volatile unsigned long*) DWT_ADDR_CTRL
#define DWT_CYCCNT  *(volatile unsigned long*) DWT_ADDR_CYCCNT
#define DWT_LAR     *(volatile unsigned long*) DWT_ADDR_LAR

// dwt helpers
static inline void init_dwt() {
    // HW needs debug enabled first! -> platform_init
    DWT_LAR = 0xC5ACCE55; // unlock DWT (not needed for qemu, but required for real hardware)
    DWT_CYCCNT = 0;
    DWT_CTRL = 1;
}

static inline unsigned long dwt_cyccnt() {
    return DWT_CYCCNT;
}