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
    switch(base){
        case 2: __printf_write_str("0b"); return;
        case 8: __printf_write_str("00"); return;
        case 16: __printf_write_str("0x"); return;
    }
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
    
    if(base_prefix) {
        print_prefix(base);
    }
    print_pad(pad_len, pad);
    __printf_write_str(p);
}

static const unsigned long pow10_tbl[] = {
    1UL,
    10UL,
    100UL,
    1000UL,
    10000UL,
    100000UL,
    1000000UL,
    10000000UL,
    100000000UL,
    1000000000UL,
};
static const unsigned long max_pow10 = (sizeof(pow10_tbl) / sizeof(pow10_tbl[0])) - 1;

static unsigned long float_to_fixed_round(float f, unsigned long scale, int *neg) {
    if (f < 0) { *neg = 1; f = -f; } 
    else { *neg = 0; }

    /* scale and round to nearest integer */
    unsigned long fixed = (unsigned long)(f * scale + 0.5f);
    return fixed;
}

static void print_float(double f, unsigned int width, unsigned int precision, char pad, int auto_trim) {
    if (precision > max_pow10)
        precision = max_pow10; // keep within pow10 table

    unsigned long scale = pow10_tbl[precision];

    /* convert to fixed point */
    int neg;
    unsigned long ufixed = float_to_fixed_round((float)f, scale, &neg);

    /* apply rounding for negative numbers properly */
    long fixed = neg ? -(long)ufixed : (long)ufixed;

    /* split integer and fraction after rounding */
    unsigned long int_part  = (fixed >= 0) ? fixed / scale : (-fixed) / scale;
    unsigned long frac_part = (fixed >= 0) ? fixed % scale : (-fixed) % scale;

    /* convert integer part */
    char ibuf[32];
    char* iend = &ibuf[31];
    *iend = 0;
    char* ip = utoa_s(int_part, 10, iend, 31);

    /* convert fraction */
    char fbuf[16];
    char* fend = &fbuf[15];
    *fend = 0;
    char* fp = fend;

    int frac_digits = precision;

    if (precision) {
        unsigned long tmp = frac_part;

        int i = 0; // c89
        for (; i < precision; i++) {
            *--fp = '0' + (tmp % 10);
            tmp /= 10;
        }

        if (auto_trim) {
            while (frac_digits && fp[frac_digits-1] == '0')
                frac_digits--;
        }
    }

    unsigned int ilen = (unsigned int)(iend - ip);
    unsigned int total =
        ilen +
        (frac_digits ? (1 + frac_digits) : 0) +
        neg;

    unsigned int pad_len = (total < width) ? width - total : 0;

    if (neg) __printf_write_char('-');
    print_pad(pad_len, pad);
    __printf_write_str(ip);

    if (frac_digits) {
        __printf_write_char('.');
        int i = 0;
        for (; i < frac_digits; i++)
            __printf_write_char(fp[i]);
    }
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
        int precision = -1;

        /* flags */
        if (*fmt == '#') { base_prefix = 1; fmt++; }
        if (*fmt == '0') { pad = '0'; fmt++; }

        /* width */
        while (*fmt >= '0' && *fmt <= '9') {
            width = width * 10 + (*fmt - '0');
            fmt++;
        }

        if (*fmt == '.') {
            fmt++;
            precision = 0;

            while (*fmt >= '0' && *fmt <= '9') {
                precision = precision * 10 + (*fmt - '0');
                fmt++;
            }
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
                print_int(&v, 1, 16, width, pad, base_prefix);
                break;
            }

            case 'f': {
                double v = va_arg(args, double);

                int auto_trim = 0;

                if (precision < 0) {
                    precision = max_pow10;
                    auto_trim = 1;
                }

                print_float(v, width, precision, pad, auto_trim);
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