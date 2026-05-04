#include "platform.h"

#include <stddef.h>
#include <sys/types.h>

#include "stm32f4xx.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_usart.h"

extern void _putchar(char character);
extern char _getchar();

__attribute__((section(".noinit"))) volatile uint32_t dfu_request_flag;
#define DFU_MAGIC 0xDEADBEEF

static void request_dfu_mode(void) {
    dfu_request_flag = DFU_MAGIC;

    // Ensure it's written before reset
    __DSB();
    __ISB();

    NVIC_SystemReset();
}

const char cmd[] = "RSTDFU\n";
uint8_t cmd_idx = 0;
int stm32_uart_available();
char stm32_uart_getc();
void stm32_uart_putc(char c);

__attribute__((noreturn))
void _exit(int status)  {
    while(1) {
        if (!stm32_uart_available()){
            continue;
        }
        const char c = stm32_uart_getc();
        stm32_uart_putc(c);
        if (c == cmd[cmd_idx]) {
            cmd_idx++;
            if (cmd_idx == sizeof(cmd) - 1) {
                request_dfu_mode();
            }
        } else {
            cmd_idx = 0;
        }
    }
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

ssize_t _read(int fd, void *buf, size_t len){
    if (fd != 0)
        return -1;
    char *cbuf = buf;
    size_t i = 0;

    while (i < len) {
        char ch = _getchar();
        if (ch == '\r') ch = '\n';  // normalize CR to LF
        cbuf[i++] = ch & 0xFF;
        if (ch == '\n') break;      // stop at newline
    }
    return i;
}



static void stm32_clk_init_hse() {
    // enable HighSpeedExternal clk and wait until ready
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));
}

static void stm32_clk_flash_init_3ws_idcache() {
    /* Flash latency (needed for 100MHz) */
    FLASH->ACR =
        /* enable instruction cache */
        FLASH_ACR_ICEN |
        /* enable data cache */
        FLASH_ACR_DCEN |
        /* waitstages for flash, access latency ~30ns */
        FLASH_ACR_LATENCY_3WS;
}

static void stm32_clk_init_pll() {
    // enable PLL and wait until ready
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));
}

static void stm32_clk_use_pll() {
    // switch system clk to PLL and wait until ready
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

static void stm32_clk_init_100MHz() {
    // configure PLL
    RCC->PLLCFGR =
        /* PLLM -> HSE clk base frequency (actually a divider to reach 1Mhz -> then any multiplication is integer)*/
        /* PLLM = F_HSE = 25MHz */
        (25 << RCC_PLLCFGR_PLLM_Pos) |
        /* PLLN -> intermediate clk target frequency */
        /* PLLN = 200MHz */
        (200 << RCC_PLLCFGR_PLLN_Pos) |
        /* PLLP -> cpu clk target frequency divider */
        /* F_CPU = 100MHz, PLLP = 200MHz / 100MHz = 2 */
        (LL_RCC_PLLP_DIV_2 << RCC_PLLCFGR_PLLP_Pos) |
        /* PLLQ -> usb clk target frequency divider */
        /* unused, but 4 is a valid value */
        (4 << RCC_PLLCFGR_PLLQ_Pos) |
        RCC_PLLCFGR_PLLSRC_HSE;
    
    // prepare flash for high frequency
    stm32_clk_flash_init_3ws_idcache();
    // enable PLL
    stm32_clk_init_pll();

    RCC->CFGR =
        RCC_CFGR_HPRE_DIV1 |    // AHB = 100MHz
        RCC_CFGR_PPRE1_DIV2 |    // APB1 = 50MHz (max 50MHz!)
        RCC_CFGR_PPRE2_DIV1;     // APB2 = 100MHz
    
    // switch system clk to pll
    stm32_clk_use_pll();
}

static void stm32_clk_init_96MHz() {
    // configure PLL
    RCC->PLLCFGR =
        /* PLLM -> HSE clk base frequency (actually a divider to reach 1Mhz -> then any multiplication is integer)*/
        /* PLLM = F_HSE = 25MHz */
        (25 << RCC_PLLCFGR_PLLM_Pos) |
        /* PLLN -> intermediate clk target frequency */
        /* PLLN = 192MHz */
        (192 << RCC_PLLCFGR_PLLN_Pos) |
        /* PLLP -> cpu clk target frequency divider */
        /* F_CPU = 96MHz, PLLP = 192MHz / 96MHz = 2, 2 as bit encoded = 0, (2->0, 4->1, 6->2, 8->3) */
        (0 << RCC_PLLCFGR_PLLP_Pos) |
        /* PLLQ -> usb clk target frequency divider */
        /* F_USB = 48MHz, PLLP = 192MHz / 48MHz = 4, as is */
        (4 << RCC_PLLCFGR_PLLQ_Pos) |
        RCC_PLLCFGR_PLLSRC_HSE;

    // prepare flash for high frequency
    stm32_clk_flash_init_3ws_idcache();
    // enable PLL
    stm32_clk_init_pll();


    RCC->CFGR =
        RCC_CFGR_HPRE_DIV1 |    // AHB = 96MHz
        RCC_CFGR_PPRE1_DIV2 |    // APB1 = 48MHz (max 50MHz!)
        RCC_CFGR_PPRE2_DIV1;     // APB2 = 96MHz
    
    // switch system clk to pll
    stm32_clk_use_pll();
}

static int uart_init_ready = 0;
static void stm32_uart_init() {
    // enable clocks
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

    // GPIO config (PA9/PA10)
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_ALTERNATE);

    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_VERY_HIGH);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_10, LL_GPIO_SPEED_FREQ_VERY_HIGH);

    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_PUSHPULL);

    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_10, LL_GPIO_PULL_UP);

    // alternate function AF7 (USART1)
    LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_9, LL_GPIO_AF_7);
    LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_10, LL_GPIO_AF_7);

    // baudrate
    LL_USART_SetBaudRate(USART1, 100000000, LL_USART_OVERSAMPLING_16, 115200);

    // enable UART
    LL_USART_Enable(USART1);
    LL_USART_EnableDirectionTx(USART1);
    LL_USART_EnableDirectionRx(USART1);

    uart_init_ready = 1;
}

int stm32_uart_available() {
    return uart_init_ready && (USART1->SR & USART_SR_RXNE);
}

char stm32_uart_getc() {
    if (uart_init_ready && stm32_uart_available()) {
        return (uint8_t)USART1->DR;
    }
    return -1; // no data
}

void stm32_uart_putc(char c) {
    if (!uart_init_ready) {
        return;
    }
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = c;
}

static void stm32_debug_led_init() {
    // Debug: enable PortC reg
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13); // Turn on LED
}

static void stm32_init_for_uart() {
    stm32_debug_led_init();
    // enable HSE
    stm32_clk_init_hse();
    // setup 100MHz clk
    stm32_clk_init_100MHz();
    SystemCoreClock = 100000000UL; // manual assignment instead of SystemCoreClockUpdate();

    stm32_uart_init();
    LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
    
}

static void stm32_init_for_usb_cdc() {
    // enable HSE
    stm32_clk_init_hse();
    // setup 96MHz clk
    stm32_clk_init_96MHz();

    SystemCoreClockUpdate();
    /*
    HAL_Init();

    MX_USB_OTG_FS_PCD_Init();

    USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);
    USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);
    USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);
    USBD_Start(&hUsbDeviceFS);
    */
    stm32_debug_led_init();
}

#define BOOTLOADER_ADDRESS 0x1FFF0000U
typedef void (*pFunction)(void);

void platform_preinit() {
    if (dfu_request_flag == DFU_MAGIC) {
        dfu_request_flag = 0; // clear so we don’t loop forever

        __set_MSP(*(volatile uint32_t*)BOOTLOADER_ADDRESS);
        ((pFunction) (*(volatile uint32_t*) (BOOTLOADER_ADDRESS + 4)))();
        while(1); // should never be reached
    }
}

void platform_init() {
    stm32_init_for_uart();
    // enable debug for dwt
    CoreDebug->DEMCR |= 0x01000000;
}