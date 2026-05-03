#include "platform.h"
#include "semihosting.h"

#include <stddef.h>
#include <sys/stat.h>
#include <sys/types.h>

__attribute__((noreturn))
void _exit(int status)  {
    semihost_exit_ext(status);
}

ssize_t _write(int fd, const void *buf, size_t len){
    if (fd == 1 || fd == 2) {
        const char *c = buf;
        for(size_t i = 0; i < len; i++) {
            semihost_call(SEMI_SYS_WRITEC, (void*)&c[i]);
        }
        return len;
    }
    return -1;
}

/*
    Semihosting readline, returns on \r or buf_len only
    meant for user input 
*/
ssize_t _read(int fd, void *buf, size_t len){
    if (fd != 0)
        return -1;
    char *cbuf = buf;
    size_t i = 0;

    while (i < len) {
        int ch = semihost_call(SEMI_SYS_READC, NULL);
        if (ch == '\r') ch = '\n';  // normalize CR to LF
        cbuf[i++] = ch & 0xFF;
        if (ch == '\n') break;      // stop at newline
    }
    return i;
}