#include "boot.h"
#include "printf.h"

extern int main();

#include "stm32f4xx.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_usart.h"

void RESET_handler() {
    platform_preinit();
    // boot
    boot_copy_data();
    boot_zero_bss();
    boot_heap_init();
    boot_call_init_array();
    //~boot

    platform_init();
    // Boot setup complete
    printf("\n\n"); // make some space to the potential previous garbage
    init_dwt(); // timing init -> setup and enable cycle count register

    printf("ISP: %#x\n", __stack_top__);
    printf("MSP: %#x\n", get_msp());
    printf("Flash: begin: 0x%#x, used: 0x%#x / 0x%x\n", __flash_start__, __flash_end__ - __flash_start__, __flash_size__);
    printf("SRAM: begin: 0x%#x, size: 0x%#x\n", __sram_start__, __sram_size__);
    printf("SRAM used: 0x%#x, available: 0x%#x\n", __heap_start__ - __sram_start__, __stack_top__ - __heap_start__);
    printf("HEAP at: 0x%#x, STACK at: 0x%#x\n", __heap_start__, __stack_top__);
    
    printf("Entering Main\n");
    // run application
    main();
    printf("Finished Main!\n");
    printf("EXIT\n");
    boot_call_fini_array();
    // shut down
    exit(0);
}

void NMI_handler() {
    printf("NMI_handler\n");
    exit(1);
}

void HardFault_Debug(unsigned int *stack, unsigned int exc_lr) {
    const unsigned int r0  = stack[0];
    const unsigned int r1  = stack[1];
    const unsigned int r2  = stack[2];
    const unsigned int r3  = stack[3];
    const unsigned int r12 = stack[4];
    const unsigned int lr  = stack[5];
    const unsigned int pc  = stack[6];
    const unsigned int psr = stack[7];

    volatile const unsigned int* SCB_CFSR  = (unsigned int*)0xE000ED28;
    volatile const unsigned int* SCB_HFSR  = (unsigned int*)0xE000ED2C;
    volatile const unsigned int* SCB_MMFAR = (unsigned int*)0xE000ED34;
    volatile const unsigned int* SCB_BFAR  = (unsigned int*)0xE000ED38;
    volatile const unsigned int* SCB_ICSR  = (unsigned int*)0xE000ED04;

    printf("\n================ HARDFAULT ================\n");

    printf("Active stack : %s\n",
        (exc_lr & (1 << 2)) ? "PSP (Thread)" : "MSP (Handler)");

    printf("EXC_RETURN   : %08lx\n", exc_lr);
    printf("Stack frame  : %p\n", stack);
    printf("\n--- Registers ---\n");
    printf("R0  = %#08lx\n", r0);
    printf("R1  = %#08lx\n", r1);
    printf("R2  = %#08lx\n", r2);
    printf("R3  = %#08lx\n", r3);
    printf("R12 = %#08lx\n", r12);
    printf("LR  = %#08lx\n", lr);
    printf("PC  = %#08lx\n", pc);
    printf("PSR = %#08lx\n", psr);
    printf("\n--- Fault Status ---\n");

    printf("CFSR  = %#08lx\n", *SCB_CFSR);
    printf("HFSR  = %#08lx\n", *SCB_HFSR);
    printf("MMFAR = %#08lx\n", *SCB_MMFAR);
    printf("BFAR  = %#08lx\n", *SCB_BFAR);

    unsigned int exception = (*SCB_ICSR) & 0x1FF;

    printf("\nException number: %lu\n", exception);

    printf("\n--- Stack dump ---\n");
    
    int i = 0; // c89
    for (; i < 16; i++) {
        printf("%02d: %#08lx\n", i, stack[i]);
    }

    printf("\n===========================================\n");

    printf("Faulting instruction address: %08lx\n", pc);
    printf("Use in GDB: list *0x%08lx\n", pc);
    printf("\nTerminating program.\n");

    exit(1);
    while (1); // opt.
}

__attribute__((naked)) 
void HARDFAULT_handler() {
    __asm volatile(
        "tst lr, #4        \n"
        "ite eq            \n"
        "mrseq r0, msp     \n"
        "mrsne r0, psp     \n"
        "mov r1, lr        \n"
        "b HardFault_Debug \n"
    );
}
void MEMMANAGE_handler() {
    printf("MEMMANAGE_handler\n");
    exit(1);
}
void BUSFAULT_handler() {
    printf("BUSFAULT_handler\n");
    exit(1);
}
void USAGEFAULT_handler() {
    printf("USAGEFAULT_handler\n");
    exit(1);
}
void SVCALL_handler() {
    printf("SVCALL_handler\n");
    exit(1);
}
void DEBUGMONITOR_handler() {
    printf("DEBUGMONITOR_handler\n");
    exit(1);
}
void PENDSV_handler() {
    printf("PENDSV_handler\n");
    exit(1);
}
void SYSTICK_handler() {
    printf("SYSTICK_handler\n");
    exit(1);
}
void DUMMY_handler() {
    printf("DUMMY_handler\n");
    exit(1);
}


__attribute__((section(".isp"), used)) byte* __isp = __stack_top__;
__attribute__((section(".vectors"), used)) ptr_func_t __isr_vectors[] = {
    RESET_handler,
    NMI_handler,
    HARDFAULT_handler,
    MEMMANAGE_handler,
    BUSFAULT_handler,
    USAGEFAULT_handler,
    DUMMY_handler,
    DUMMY_handler,
    DUMMY_handler,
    DUMMY_handler,
    SVCALL_handler,
    DEBUGMONITOR_handler,
    DUMMY_handler,
    PENDSV_handler,
    SYSTICK_handler,
};