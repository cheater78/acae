MEMORY
{
    FLASH(rx) : ORIGIN = 0x08000000, LENGTH = 512K /* STM32F411 requires flash at 0x08000000 */
    SRAM(rwx) : ORIGIN = 0x20000000, LENGTH = 128K /* STM32F411 for validation has 128K */
}