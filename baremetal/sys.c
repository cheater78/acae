#include "sys.h"

#include <errno.h>
#include <stddef.h>
#include <sys/reent.h>
#include <sys/stat.h>
#include <sys/types.h>

extern void _putchar(char character);

// defined by platform
extern void _exit(int status);
extern ssize_t _write(int fd, const void *buf, size_t len);
extern ssize_t _read(int fd, void *buf, size_t len);

static byte* __heap_end__ = 0;

// system backend implementations
#define STACK_GUARD 1024
void* _sbrk(ptrdiff_t incr) {
    if (__heap_end__ == 0) {
        __heap_end__ = __heap_start__;
    }

    byte* prev = __heap_end__;
    byte* next = __heap_end__ + incr;

    if (next >= (byte*)get_msp() - STACK_GUARD) {
        errno = ENOMEM;
        return (void*)-1;
    }
    __heap_end__ = next;
    return prev;
}

ssize_t _write_r(struct _reent *r, int fd, const void *buf, size_t len) {
    if (fd != 1 && fd != 2) {
        r->_errno = EBADF;
        return -1;
    }
    return _write(fd, buf, len);
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


int _kill(int pid, int sig) {
    errno = EINVAL;
    return -1;
}

int _getpid(void) {
    return 1;
}
__attribute__((weak))
void _init() {
}

__attribute__((weak))
void _fini() {
}

//~ system backend implementations

inline byte* get_heap_end() {
    return __heap_end__;
}

inline void exit(int status) {
    _exit(status);
}