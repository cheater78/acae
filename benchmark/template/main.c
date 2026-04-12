#include "bmstdlib.h"

#define BENCH_ITER 1024
void benchmark_run() {
    volatile unsigned long start_cycles = dwt_cyccnt();

    for(unsigned int i = 0; i < BENCH_ITER; i++) {
        // just using the loop as load
    }

    volatile unsigned long end_cycles = dwt_cyccnt();

    printf(
        "Template Benchmark Run, cycles[AT %#lx]: start=%lu, end=%lu, diff=%lu\n",
        DWT_ADDR_CYCCNT, start_cycles, end_cycles, tick_diff_u32(start_cycles, end_cycles)
    );
}

int main(void) {
    printf("Hello from our Benchmark template!\n");
    printf("dec: %ld\n", 1234567890UL);
    printf("hex: %#08lx\n", 0xF0A9);
    printf("float: %06.3f\n", -123.321f); // front padding not working
    
    benchmark_run();

    return 0;
}