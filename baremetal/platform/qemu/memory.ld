MEMORY
{
    FLASH(rx) : ORIGIN = 0x00000000, LENGTH = 512K /* qemu msp386 requires flash at 0x00000000 */
    SRAM(rwx) : ORIGIN = 0x20000000, LENGTH = 128K /* our model has 128K */
}