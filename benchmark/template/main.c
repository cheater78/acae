#include "bmstdlib.h"

#define BENCH_ITER 1024
void benchmark_run() {
    volatile unsigned long start_cycles = dwt_cyccnt();

    for(unsigned int i = 0; i < BENCH_ITER; i++) {
        // just using the loop as load
        volatile unsigned int suppress_opt = i;
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

    const double my_float = 54321.12345f;
    printf("05.5f: %05.5f\n", my_float);
    printf("6.3f: %6.3f\n", my_float);
    printf("0.5f: %0.5f\n", my_float);
    printf("05.f: %05.f\n", my_float);
    printf("0f: %0f\n", my_float);
    
    benchmark_run();

    return 0;
}