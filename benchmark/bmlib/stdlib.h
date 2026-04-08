#pragma once
// STD LIB replacements
#include "printf.h"

char* strcpy(char *dest, const char *src);
int strcmp(const char *s1, const char *s2);

void* malloc(unsigned long size);
void free(void* p);

// sys/times.h
typedef unsigned long clock_t;
struct tms {
    clock_t tms_utime;		/* User CPU time.  */
    clock_t tms_stime;		/* System CPU time.  */

    clock_t tms_cutime;		/* User CPU time of dead children.  */
    clock_t tms_cstime;		/* System CPU time of dead children.  */
};

clock_t times(struct tms* buffer);

