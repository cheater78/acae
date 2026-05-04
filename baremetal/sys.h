#pragma once

typedef unsigned char byte;
typedef unsigned int word;
typedef unsigned int ptr;
typedef void (*ptr_func_t)();

// define symbols as extern array -> makes the defined variable a ptr containing the linker symbol
#define SYMBOL_BYTE_PTR(__symbol_name) extern byte __symbol_name[]
#define SYMBOL_WORD_PTR(__symbol_name) extern word __symbol_name[]
#define SYMBOL_FUNC_PTR(__symbol_name) extern ptr_func_t __symbol_name[]

// sizes are the same as byte ptr -> the symbol resolves the address of the defined var, which can be a size 
// (semantics are completely arbituary and have to be known by you)
#define SYMBOL_BYTE_SIZE(__symbol_name) SYMBOL_BYTE_PTR(__symbol_name)

// linker script symbols, already converted to byte(uint8_t) or word(uint32_t) ptrs
// static ptrs and sizes
SYMBOL_BYTE_PTR(__flash_start__);
SYMBOL_BYTE_PTR(__flash_end__); // end of used flash
SYMBOL_BYTE_SIZE(__flash_size__); // size of available flash

SYMBOL_BYTE_PTR(__sram_start__);
SYMBOL_BYTE_SIZE(__sram_size__);
// static ram content
SYMBOL_WORD_PTR(__data_start__);
SYMBOL_WORD_PTR(__data_end__);
SYMBOL_WORD_PTR(__data_load__);
SYMBOL_WORD_PTR(__bss_start__);
SYMBOL_WORD_PTR(__bss_end__);
// dynamic ram content
SYMBOL_BYTE_PTR(__heap_start__);
SYMBOL_BYTE_PTR(__stack_top__);

SYMBOL_FUNC_PTR(__preinit_array_start__);
SYMBOL_FUNC_PTR(__preinit_array_end__);
SYMBOL_FUNC_PTR(__init_array_start__);
SYMBOL_FUNC_PTR(__init_array_end__);
SYMBOL_FUNC_PTR(__fini_array_start__);
SYMBOL_FUNC_PTR(__fini_array_end__);

static inline byte* get_stack_top() {
    return __stack_top__;
}
static inline __attribute__((always_inline))
byte* get_msp(void) {
    byte* sp;
    __asm__ volatile ("mrs %0, msp" : "=r"(sp));
    return sp;
}

// Free initialized heap will contain char[4]{'F','R','E','E'}
#define HEAP_FREE_PATTERN 0x45455246

static inline byte* get_heap_start() {
    return __heap_start__;
}
byte* get_heap_end();

static inline void exit(int status) {
    extern void _exit(int status);
    _exit(status);
}

#include <platform.h>
#include "dwt.h"