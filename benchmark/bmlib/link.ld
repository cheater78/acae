ENTRY(RESET_handler)

MEMORY
{
    FLASH(rx) : ORIGIN = 0x00000000, LENGTH = 512K
    SRAM(rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS
{
    __flash_start = ORIGIN(FLASH);
    __flash_size = LENGTH(FLASH);
    .text : AT (ORIGIN(FLASH)) {
        __flash_start = .;
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
        __flash_end = .;
    } >FLASH
    
    .preinit_array ALIGN(4): {
        __preinit_array_start = .;
        KEEP(*(.preinit_array))
        __preinit_array_end = .;
    } >FLASH

    .init_array ALIGN(4): {
        __init_array_start = .;
        KEEP(*(.init))
        KEEP(*(.init_array))
        __init_array_end = .;
    } >FLASH

    .fini_array ALIGN(4): {
        __fini_array_start = .;
        KEEP(*(.fini_array))
        __fini_array_end = .;
    } >FLASH
    
    __sram_start = ORIGIN(SRAM);
    __sram_size = LENGTH(SRAM);
    __data_load = LOADADDR(.data);
    . = ORIGIN(SRAM);
    .data ALIGN(4) : {
        __data_start = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end = .;
    } >SRAM

    .bss ALIGN(4) (NOLOAD) : {
        __bss_start = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end = .;
        *(.noinit)
        *(.noinit*)
    } >SRAM

    . = ALIGN(4);
    __heap_start = .;
    /* heap */

    /* stack */
    __stack_top = ORIGIN(SRAM) + LENGTH(SRAM);
}