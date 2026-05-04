#pragma once

//#define PLATFORM_QEMU 1
#define PLATFORM_NATIVE 1

// CPU frequency
#define F_CPU 100000000UL

// DWT (CYCNT)
#define _DWT_BASE_MEM_ADDR 0xE0001000

void platform_preinit();
void platform_init();