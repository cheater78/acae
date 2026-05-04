#pragma once
#include "sys.h"

// STD LIB replacements
// stdio.h
#include "printf.h"

// string.h
char* strcpy(char *dest, const char *src);
int strcmp(const char *s1, const char *s2);

// memory.h
void* malloc(unsigned long size);
void free(void* p);

// sys/times.h
#define HZ F_CPU
typedef unsigned long clock_t;
struct tms {
    clock_t tms_utime;		/* User CPU time.  */
    clock_t tms_stime;		/* System CPU time.  */

    clock_t tms_cutime;		/* User CPU time of dead children.  */
    clock_t tms_cstime;		/* System CPU time of dead children.  */
};
clock_t times(struct tms* buffer);

// custom helpers
unsigned long tick_diff(unsigned long start, unsigned long end, unsigned long tick_max);
unsigned long tick_diff_u32(unsigned long start, unsigned long end);

#define MICS_PER_SECOND 1000000UL
#define NANOS_PER_SECOND 1000000000UL
#define TICKS_TO_TIME(ticks, RES) ((HZ > RES) ? ticks / (HZ / RES) : ticks * (RES / HZ))
unsigned long ticks_to_us(unsigned long ticks);
unsigned long ticks_to_ns(unsigned long ticks);