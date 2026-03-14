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

void* malloc(unsigned long size) {
    extern void* _sbrk(ptrdiff_t incr);
    return _sbrk(size);
}
void free(void* p) {
}

clock_t times(struct tms *buffer) {
    if (buffer) {
        buffer->tms_utime  = systick();
        buffer->tms_stime  = 0;
        buffer->tms_cutime = 0;
        buffer->tms_cstime = 0;
    }

    return systick();
}