#pragma once
#include "sys.h"

static inline void boot_copy_data() {
    const word* data_src = __data_load__;
    word* data_dst = __data_start__;

    while (data_dst < __data_end__)
        *data_dst++ = *data_src++;
}

static inline void boot_zero_bss() {
    word* bss_start = __bss_start__;
    while (bss_start < __bss_end__)
        *bss_start++ = 0;
}

static inline void boot_heap_init() {
    word* heap_start = (word*)__heap_start__;
    const word* stack_ptr = (word*)get_msp();

    while (heap_start < stack_ptr)
        *heap_start++ = HEAP_FREE_PATTERN;
}

static inline void boot_call_init_array() {
    ptr_func_t* array = __preinit_array_start__;
    while (array < __preinit_array_end__) {
        (*array)();
        array++;
    }

    array = __init_array_start__;
    while (array < __init_array_end__) {
        (*array)();
        array++;
    }
}

static inline void boot_call_fini_array() {
    ptr_func_t* array = __fini_array_start__;
    while (array < __fini_array_end__) {
        (*array)();
        array++;
    }
}

// SYSTICK
static inline void boot_SYSTICK_init() {
    SYST_RVR = 0xFFFFFF;
    SYST_CVR = 0;
    SYST_CSR = 5; // enable + CPU clock
}
