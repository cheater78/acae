#include "semihosting.h"

// portable printf impl
void __printf_write_char(char c) {
    semihost_write_char(c);
}
void __printf_write_str(const char* str){
    semihost_write_str(str);
}