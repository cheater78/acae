#include "platform.h"

void clock_init(void) {
    /* Enable HSE */
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));

    /* Configure PLL */
    RCC->PLLCFGR =
        (25 << RCC_PLLCFGR_PLLM_Pos) |   // PLLM
        (192 << RCC_PLLCFGR_PLLN_Pos) |  // PLLN
        (2 << RCC_PLLCFGR_PLLP_Pos) |    // PLLP = /2
        (4 << RCC_PLLCFGR_PLLQ_Pos) |    // PLLQ = /4
        RCC_PLLCFGR_PLLSRC_HSE;

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));

    /* Flash latency (needed for 100 MHz) */
    FLASH->ACR =
        FLASH_ACR_ICEN |
        FLASH_ACR_DCEN |
        FLASH_ACR_LATENCY_3WS;

    /* Switch system clock to PLL */
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    SystemCoreClock = 96000000UL;
}

void platform_init() {
    HAL_Init();

    // clk at 48MHz for USB -> FCPU = 96MHz
    clock_init();
    
    MX_USB_OTG_FS_PCD_Init();

    USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);
    USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);
    USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);
    USBD_Start(&hUsbDeviceFS);

    // Debug: enable PortC reg
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
}



__attribute__((noreturn))
void _exit(int status)  {
    while(1);
}

ssize_t _write(int fd, const void *buf, size_t len){
    if (fd == 1 || fd == 2) {
        const char *c = buf;
        for(size_t i = 0; i < len; i++) {
            _putchar(c[i]);
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
        int ch = 0; // TODO: UART getc
        if (ch == '\r') ch = '\n';  // normalize CR to LF
        cbuf[i++] = ch & 0xFF;
        if (ch == '\n') break;      // stop at newline
    }
    return i;
}