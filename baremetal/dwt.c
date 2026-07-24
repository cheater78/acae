#include "dwt.h"

#include "printf.h"
#include "stm32f4xx.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_usart.h"

void init_dwt() {
    // HW needs debug enabled first! -> platform_init
    DWT_LAR = 0xC5ACCE55; // unlock DWT (not needed for qemu, but required for real hardware)
    DWT_CYCCNT = 0;
    DWT_CTRL |= (1 << 0);
    
    printf("DEMCR = %08lx\n", CoreDebug->DEMCR);
    printf("DWT_CTRL = %08lx\n", DWT_CTRL);
    printf("DWT_CYCCNT = %08lx\n", DWT_CYCCNT);
}