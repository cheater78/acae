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
void init_dwt();

// moved to a real symbol for acae's detection in qemu -> will cause some call overhead (guestimate: 3-5 cycles + pipline)
unsigned long dwt_cyccnt();