#include "dwt.h"
#include "printf.h"
#include "stdlib.h"
#include "sys.h"

int main(void) {
    printf("Hello from our Benchmark template!\n");
    printf("dec: %ld\n", 1234567890UL);
    printf("hex: %#08lx\n", 0xF0A9);
    printf("float: %06.3f\n", -123.321f); // front padding not working
    
    static const unsigned int iterations = 1000000; 
    const unsigned int start = DWT_CYCCNT;
    for(unsigned int iter = 0; iter < iterations; iter++) {
        const unsigned int passed = DWT_CYCCNT - start;
        if (iter % (iterations / 10) == 0) {
            printf("Iteration: %u, Cycles: %u, Cycles/Iter: %u\n", iter, passed, iter ? (passed / iter) : 0);
        }
    }

    return 0;
}