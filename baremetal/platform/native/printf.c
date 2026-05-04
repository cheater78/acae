
char _getchar() {
    extern char stm32_uart_getc();
    return stm32_uart_getc();
}

void _putchar(char character) {
    extern void stm32_uart_putc(char c);
    stm32_uart_putc(character);
}