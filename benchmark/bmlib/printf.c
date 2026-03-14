#include "printf.h"

#include <stdarg.h>

extern void __printf_write_char(char);
extern void __printf_write_str(const char*);

// portable printf lib

static char* utoa_s(unsigned long v, const unsigned int base, char* buf_end, unsigned int buf_size) {
    static const char digits[] = "0123456789ABCDEF";
    char* p = buf_end; // one after the last elem

    while (v && buf_size) {
        *--p = digits[v % base]; // precrem
        v /= base;
        buf_size--;
    };

    return p;
}

static char* itoa_s(long v, const unsigned int base, char* buf_end, unsigned int buf_size) {
    unsigned long u = (unsigned long)v;

    if (v < 0) {
        u = ~u + 1;  // safe two's complement negate
        char* p = utoa_s(u, base, buf_end, buf_size);
        *--p = '-';
        return p;
    }

    return utoa_s(u, base, buf_end, buf_size);
}

static void print_pad(unsigned int len, char pad) {
    while (len) {
        __printf_write_char(pad);
        len--;
    }
}

static void print_prefix(unsigned int base) {
    static const char* prefixes[] = {"0b", "", "00", "0x"};
    static const unsigned int supported_bases = 0b00011010;
    if (!(base & supported_bases))
        return;
    unsigned int base_ld = 0;
    while(base >> 1) {
        base = base >> 1;
        base_ld++;
    }
    __printf_write_str(prefixes[base_ld - 1]);
}

static void print_int(void* v, unsigned int is_unsigned, unsigned int base, unsigned int width, char pad, unsigned int base_prefix) {
    static const unsigned int buf_len = 33; // max 32bit in 0b + \0
    static const unsigned int str_buf_len = buf_len - 1;

    char buf[buf_len];
    char* str_end = &buf[str_buf_len];
    *str_end = 0; // null term
    
    const char* p = (is_unsigned) ?
        utoa_s(*(unsigned long*)v, base, str_end, str_buf_len):
        itoa_s(*(  signed long*)v, base, str_end, str_buf_len);

    const unsigned int total_len = (unsigned int)(str_end - p) + ((base_prefix) ? 2 : 0);
    const unsigned int pad_len = (total_len < width) ? (int)width - (int)total_len : 0;
    print_pad(pad_len, pad);
    if(base_prefix) print_prefix(base);
    __printf_write_str(p);
}

void __printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt != '%') {
            if (*fmt == '\n') __printf_write_str("\r\n");
            else __printf_write_char(*fmt);
            fmt++;
            continue;
        }

        fmt++;

        unsigned int base_prefix = 0;
        int width = 0;
        int longflag = 0;
        char pad = ' ';

        /* flags */
        if (*fmt == '#') { base_prefix = 1; fmt++; }
        if (*fmt == '0') { pad = '0'; fmt++; }

        /* width */
        while (*fmt >= '0' && *fmt <= '9') {
            width = width * 10 + (*fmt - '0');
            fmt++;
        }

        /* length modifier */
        if (*fmt == 'l') { longflag = 1; fmt++; }

        switch (*fmt) {
            case 'c': {
                char c = (char)va_arg(args, int);
                __printf_write_char(c);
                break;
            }

            case 's': {
                const char *s = va_arg(args, const char*);
                __printf_write_str(s);
                break;
            }

            case 'd': {
                long v = longflag ? va_arg(args, long) : va_arg(args, int);
                print_int(&v, 0, 10, width, pad, base_prefix);
                break;
            }

            case 'u': {
                unsigned long v = longflag ? va_arg(args, unsigned long) : va_arg(args, unsigned int);
                print_int(&v, 1, 10, width, pad, base_prefix);
                break;
            }

            case 'x': {
                unsigned long v = longflag ? va_arg(args, unsigned long) : va_arg(args, unsigned int);
                print_int(&v, 1, 16, width, pad, base_prefix);
                break;
            }

            case 'p': {
                unsigned long v = (unsigned long)va_arg(args, void*);
                print_int(&v, 1, 10, width, pad, base_prefix);
                break;
            }

            case '%':
                __printf_write_char('%');
                break;

            default:
                __printf_write_char('%');
                __printf_write_char(*fmt);
                break;
        }

        fmt++;
    }

    va_end(args);
}