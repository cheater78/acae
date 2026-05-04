#pragma once 
#include <stdbool.h>

#define USE_QEMU_CA_PLUGIN_DEBUG_LOG 0

void pllog_init(const char* path, bool overwrite);
void pllog_fini(void);

void pllog(const char* fmt, ...);