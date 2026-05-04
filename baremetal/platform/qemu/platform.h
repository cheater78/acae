#pragma once

// CPU frequency
#define F_CPU 100000000UL

// DWT (CYCNT)
#define _DWT_BASE_MEM_ADDR 0x40000000

void platform_preinit();
void platform_init();