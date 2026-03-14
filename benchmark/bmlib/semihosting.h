#define SEMI_SYS_OPEN           0x01
#define SEMI_SYS_CLOSE          0x02
#define SEMI_SYS_WRITEC         0x03
#define SEMI_SYS_WRITE0         0x04
#define SEMI_SYS_WRITE          0x05
#define SEMI_SYS_READ           0x06
#define SEMI_SYS_READC          0x07
#define SEMI_SYS_ISERROR        0x08
#define SEMI_SYS_ISTTY          0x09
#define SEMI_SYS_SEEK           0x0A
#define SEMI_SYS_FLEN           0x0C
#define SEMI_SYS_TMPNAM         0x0D
#define SEMI_SYS_REMOVE         0x0E
#define SEMI_SYS_RENAME         0x0F
#define SEMI_SYS_CLOCK          0x10
#define SEMI_SYS_TIME           0x11
#define SEMI_SYS_ERRNO          0x13
#define SEMI_SYS_GET_CMDLINE    0x15
#define SEMI_SYS_HEAPINFO       0x16
#define SEMI_SYS_EXIT           0x18
#define SEMI_SYS_EXIT_EXTENDED  0x20

#define ADP_STOPPED_APPLICATION_EXIT 0x20026

static inline int semihost_call(int op, void* arg) {
    register int r0 asm("r0") = op;
    register void *r1 asm("r1") = arg;

    asm volatile (
        "bkpt 0xAB"
        : "+r"(r0)
        : "r"(r1)
        : "memory"
    );

    return r0;
}

static inline void semihost_write_char(char c) {
    semihost_call(SEMI_SYS_WRITEC, &c);
}

static inline void semihost_write_str(const char *s) {
    semihost_call(SEMI_SYS_WRITE0, (void *)s);
}

static inline int semihost_read_char(void) {
    return semihost_call(SEMI_SYS_READC, 0);
}

__attribute__((noreturn))
static inline void semihost_exit_ext(int status) {
    _Static_assert(sizeof(int) == 4, "Semihosting ABI expects 32-bit int");

    struct {
        int reason;
        int subcode;
    } param = {
        ADP_STOPPED_APPLICATION_EXIT,
        status
    };

    semihost_call(SEMI_SYS_EXIT_EXTENDED, &param);

    __builtin_unreachable();
}

__attribute__((noreturn))
static inline void semihost_exit_legacy(int reason) {
    semihost_call(SEMI_SYS_EXIT, (void*)reason);
    __builtin_unreachable();
}