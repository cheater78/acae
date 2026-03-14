#pragma once
// printf implementation if stdio not present
#ifdef __cplusplus
extern "C" {
#endif

// portable printf lib
void __printf(const char *fmt, ...);

#ifndef printf
#define printf __printf
#endif

#ifdef __cplusplus
}
#endif