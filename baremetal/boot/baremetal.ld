ENTRY(RESET_handler)

MEMORY
{
    FLASH(rx) : ORIGIN = 0x08000000, LENGTH = 512K /* STM32F411 requires flash at 0x08000000 */
    SRAM(rw) : ORIGIN = 0x20000000, LENGTH = 128K /* STM32F411 for validation has 128K */
}

SECTIONS
{
    __flash_start__ = ORIGIN(FLASH);
    __flash_size__ = LENGTH(FLASH);
    .text : AT (ORIGIN(FLASH)) {
        __flash_start__ = .;
        KEEP(*(.isp)) /* initial stack ptr */
        KEEP(*(.vectors))
        KEEP(*(.vectors*))
        . = ALIGN(4);
        *(.text*)
        . = ALIGN(4);
        KEEP(*(.rodata))
        *(.rodata*)
        . = ALIGN(4);
        KEEP(*(.ARM.exidx*))
        . = ALIGN(4);
        KEEP(*(.eh_frame*))
        __flash_end__ = .;
    } >FLASH
    
    .preinit_array ALIGN(4): {
        __preinit_array_start__ = .;
        KEEP (*(SORT(.preinit_array.*)))
        KEEP (*(.preinit_array))
        __preinit_array_end__ = .;
    } > FLASH

    .init_array ALIGN(4): {
        __init_array_start__ = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        __init_array_end__ = .;
    } > FLASH

    .fini_array ALIGN(4): {
        __fini_array_start__ = .;
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array))
        __fini_array_end__ = .;
    } > FLASH
    
    __sram_start__ = ORIGIN(SRAM);
    __sram_size__ = LENGTH(SRAM);
    __data_load__ = LOADADDR(.data);
    . = ORIGIN(SRAM);
    .data ALIGN(4) : {
        __data_start__ = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end__ = .;
    } >SRAM

    .bss ALIGN(4) (NOLOAD) : {
        __bss_start__ = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
        *(.noinit)
        *(.noinit*)
    } >SRAM

    . = ALIGN(4);
    __heap_start__ = .;
    /* heap */

    /* stack */
    __stack_top__ = ORIGIN(SRAM) + LENGTH(SRAM);
}