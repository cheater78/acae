#pragma once

// CPU frequency
#define F_CPU 100000000UL

// DWT (CYCNT)
// no enbling needed for QEMU, it's always on and mapped at _DWT_ADDR_QEMU_BASE
#define DWT_ENABLE

#define _DWT_BASE_MEM_ADDR 0x40000000

