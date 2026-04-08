#include "stdlib.h"
#include "sys.h"

#include <stddef.h>

// STD

char* strcpy(char *dest, const char *src) {
    char *d = dest;

    while (*src) {
        *d++ = *src++;
    }

    *d = '\0';   // null terminate
    return dest;
}
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }

    return (unsigned char)*s1 - (unsigned char)*s2;
}

#include "printf.h"
void* malloc(unsigned long size) {
    extern void* _sbrk(ptrdiff_t incr);
    size = (size + 7) & ~7; // align
    void* new_heap_max = _sbrk(size);
    printf("Malloc ptr: %#lx, end: %#lx, MSP: %#lx, H/S distance: %#lx\n", new_heap_max, new_heap_max + size, get_msp(), (void*)get_msp() - (new_heap_max + size));
    return new_heap_max;
}
void free(void* p) {
}

clock_t times(struct tms *buffer) {
    const clock_t utime = DWT_CYCCNT / (F_CPU / 1000000UL); // time as µs from clk cycles
    if (buffer) {
        buffer->tms_utime  = utime;
        buffer->tms_stime  = 0;
        buffer->tms_cutime = 0;
        buffer->tms_cstime = 0;
    }

    return utime;
}