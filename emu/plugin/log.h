#pragma once 
#include <stdbool.h>

void pllog_init(const char* path, bool overwrite);
void pllog_fini(void);

void pllog(const char* fmt, ...);