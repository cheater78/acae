#include "sys.h"
#include "semihosting.h"

#include <errno.h>
#include <stddef.h>
#include <sys/reent.h>
#include <sys/stat.h>
#include <sys/types.h>

static byte* __heap_end = 0;

// system backend implementations
#define STACK_GUARD 1024
void* _sbrk(ptrdiff_t incr) {
    if (__heap_end == 0) {
        __heap_end = __heap_start;
    }

    byte* prev = __heap_end;
    byte* next = __heap_end + incr;

    if (next >= (byte*)get_msp() - STACK_GUARD) {
        errno = ENOMEM;
        return (void*)-1;
    }
    __heap_end = next;
    return prev;
}

ssize_t _write(int fd, const void *buf, size_t len){
    if (fd == 1 || fd == 2) {
        const char *c = buf;
        size_t i = 0; // c89
        for(; i < len; i++) {
            semihost_call(SEMI_SYS_WRITEC, (void*)&c[i]);
        }
        return len;
    }
    return -1;
}

ssize_t _write_r(struct _reent *r, int fd, const void *buf, size_t len) {
    if (fd != 1 && fd != 2) {
        r->_errno = EBADF;
        return -1;
    }
    return _write(fd, buf, len);
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

ssize_t _read_r(struct _reent *r, int fd, void *buf, size_t len) {
    if (fd != 0) {
        r->_errno = EBADF;
        return -1;
    }
    return _read(fd, buf, len);
}

int _close(int file) {
    errno = EBADF;
    return -1;
}
int _close_r(struct _reent *r, int fd) { (void)r; return _close(fd); }

off_t _lseek(int file, off_t ptr, int dir) {
    errno = ESPIPE;
    return -1;
}
int _lseek_r(struct _reent *r, int fd, int ptr, int dir) { (void)r; return _lseek(fd, ptr, dir); }

int _fstat(int file, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}
int _fstat_r(struct _reent *r, int fd, void *st) { (void)r; (void)fd; (void)st; return _fstat(fd, st); }

int _isatty(int file) {
    return (file <= 2) ? 1 : 0;
}
int _isatty_r(struct _reent *r, int fd) { (void)r; return _isatty(fd); }

__attribute__((noreturn))
void _exit(int status)  {
    semihost_exit_ext(status);
}

int _getpid(void) { return 1; }

void _init() {
}
void _fini() {
}

//~ system backend implementations

inline byte* get_heap_end() {
    return __heap_end;
}

// SYSTICK
unsigned long systick() {
    static unsigned int last = 0;
    unsigned int curr = 0xFFFFFF - SYST_CVR;
    
    if(curr < last) {
        curr = 0xFFFFFF - (last - curr);
    }
    
    return last = curr;
}