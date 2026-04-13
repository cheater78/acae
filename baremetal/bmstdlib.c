#include "bmstdlib.h"
#include "sys.h"
#include <stddef.h>
// STD LIB replacements
// string.h
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
// memory.h
void* malloc(unsigned long size) {
    extern void* _sbrk(ptrdiff_t incr);
    size = (size + 7) & ~7; // align
    void* new_heap_max = _sbrk(size);
    printf("Malloc ptr: %#lx, end: %#lx, MSP: %#lx, H/S distance: %#lx\n", new_heap_max, new_heap_max + size, get_msp(), (void*)get_msp() - (new_heap_max + size));
    return new_heap_max;
}
void free(void* p) {
}
// sys/times.h
clock_t times(struct tms *buffer) {
    if (buffer) {
        buffer->tms_utime  = dwt_cyccnt();
        buffer->tms_stime  = 0;
        buffer->tms_cutime = 0;
        buffer->tms_cstime = 0;
    }
    printf("std/times: %lu ticks\n", buffer->tms_utime);
    return buffer->tms_utime;
}

// custom helpers
unsigned long tick_diff(unsigned long start, unsigned long end, unsigned long tick_max) {
    if (end >= start) {
        return end - start;
    } else {
        return tick_max - (start - end); // correct OF for one phase (best we can do)
    }
}
unsigned long tick_diff_u32(unsigned long start, unsigned long end) {
    return tick_diff(start, end, 0xFFFFFFFF);
}

unsigned long ticks_to_us(unsigned long ticks) {
    return ticks / (HZ / MICS_PER_SECOND); 
}