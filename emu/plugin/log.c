#include "log.h"
#include <stdio.h>
#include <stdarg.h>

static FILE* log_file = NULL;

void pllog_init(const char *path, bool overwrite) {
    if (log_file) {
        fclose(log_file);
    }

    log_file = fopen(path, overwrite ? "w" : "a");
    if (!log_file) {
        // fallback: optionally handle error
        perror("pllog_init: log file open failed!");
    }
}

void pllog_fini(void) {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

void pllog(const char *fmt, ...) {
    if (!log_file) {
        perror("pllog: log file not initialized!");
    }

    va_list args;
    va_start(args, fmt);
    vfprintf(log_file, fmt, args);
    va_end(args);

    fflush(log_file); //TODO: omit for performance?!
}