#include "bmstdlib.h"

#ifndef ITERATIONS
#define ITERATIONS 1000000UL
#endif

void benchmark_run(const unsigned long iterations) {
    volatile const unsigned long start_cycles = dwt_cyccnt();

    for(unsigned int i = 0; i < iterations; i++) {
        // just using the loop as load
        volatile unsigned int suppress_opt = i;
    }

    volatile const unsigned long end_cycles = dwt_cyccnt();

    const unsigned long total_cycles = tick_diff_u32(start_cycles, end_cycles);
    const unsigned long total_cycles_per_iteration = total_cycles / iterations;
    const unsigned long total_time_us = ticks_to_us(total_cycles);
    const unsigned long total_time_ns_per_iteration = ticks_to_ns(total_cycles) / iterations;

    printf("Template Benchmark Run finished!\n");
    printf("  cycles[AT %#lx]: start=%lu, end=%lu, diff=%lu, cycles_per_iteration=%lu\n",
        DWT_ADDR_CYCCNT, start_cycles, end_cycles, total_cycles, total_cycles_per_iteration);
    printf("  equivalent real: time=%luus, time/iter=%luns\n",
        total_time_us, total_time_ns_per_iteration);
}

int main(void) {
    printf("Hello from our Benchmark template!\n");

    benchmark_run(ITERATIONS);

    return 0;
}