#pragma once

#define PLATFORM_QEMU 1
//#define PLATFORM_NATIVE 1

// CPU frequency
#define F_CPU 100000000UL

// DWT (CYCNT)
#ifdef PLATFORM_QEMU
    // no enbling needed for QEMU, it's always on and mapped at _DWT_ADDR_QEMU_BASE
    #define DWT_ENABLE
#elif defined(PLATFORM_NATIVE)
    // on native, we need to enable the DWT unit
    #define SCB_DEMCR   *(volatile unsigned int*) 0xE000EDFC
    #define DWT_ENABLE SCB_DEMCR |= 0x01000000
#endif

#define _DWT_ADDR_SPACE_SIZE 0x1000
#define _DWT_ADDR_NATIVE_BASE 0xE0001000
#define _DWT_ADDR_QEMU_BASE 0x40000000

#define _DWT_BASE_MEM_ADDR _DWT_ADDR_QEMU_BASE

#define DWT_ADDR_CTRL       (_DWT_BASE_MEM_ADDR + 0x000)
#define DWT_ADDR_CYCCNT     (_DWT_BASE_MEM_ADDR + 0x004)
#define DWT_ADDR_LAR        (_DWT_BASE_MEM_ADDR + 0xFB0)

// dwt registers
#define DWT_CTRL    *(volatile unsigned long*) DWT_ADDR_CTRL
#define DWT_CYCCNT  *(volatile unsigned long*) DWT_ADDR_CYCCNT
#define DWT_LAR     *(volatile unsigned long*) DWT_ADDR_LAR
