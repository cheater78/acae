#include "semihosting.h"

void _putchar(char character) {
    semihost_write_char(character);
}