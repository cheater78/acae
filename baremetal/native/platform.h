#pragma once

#define PLATFORM_QEMU 1
//#define PLATFORM_NATIVE 1

// CPU frequency
#define F_CPU 100000000UL

// DWT (CYCNT)
// on native, we need to enable the DWT unit
#define SCB_DEMCR   *(volatile unsigned int*) 0xE000EDFC
#define DWT_ENABLE SCB_DEMCR |= 0x01000000

#define _DWT_BASE_MEM_ADDR 0xE0001000

//TODO: coarse template for now, impl properly for native
void clock_init(void) {
    // Enable HSE (external 25 MHz crystal)
    RCC->CR |= (1 << 16);
    while (!(RCC->CR & (1 << 17))); // wait HSERDY

    // Configure FLASH latency (3 wait states for ~100 MHz)
    volatile uint32_t *FLASH_ACR = (uint32_t*)(FLASH_BASE + 0x00);
    *FLASH_ACR = (3 << 0) | (1 << 8) | (1 << 9); // latency + cache

    // PLL config:
    // PLLM = 25
    // PLLN = 192
    // PLLP = 2  → 96 MHz SYSCLK
    // PLLQ = 4  → 48 MHz USB
    RCC->PLLCFGR =
        (25 << 0) |        // PLLM
        (192 << 6) |       // PLLN
        (0 << 16) |        // PLLP = 2
        (1 << 22) |        // HSE as source
        (4 << 24);         // PLLQ

    // Enable PLL
    RCC->CR |= (1 << 24);
    while (!(RCC->CR & (1 << 25))); // PLLRDY

    // Switch SYSCLK to PLL
    RCC->CFGR |= (2 << 0);
    while (((RCC->CFGR >> 2) & 3) != 2);
}