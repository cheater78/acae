#pragma once

#include "stm32f4xx.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"

#define PLATFORM_QEMU 1
//#define PLATFORM_NATIVE 1

// CPU frequency
#define F_CPU 100000000UL

// DWT (CYCNT)
// on native, we need to enable the DWT unit
// #define SCB_DEMCR   *(volatile unsigned int*) 0xE000EDFC
#define DWT_ENABLE CoreDebug->DEMCR |= 0x01000000

#define _DWT_BASE_MEM_ADDR 0xE0001000

void platform_init();