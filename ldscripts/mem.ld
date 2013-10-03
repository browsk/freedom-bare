/* Linker script to configure memory regions. 
 * Need modifying for a specific board. 
 *   FLASH.ORIGIN: starting address of flash
 *   FLASH.LENGTH: length of flash
 *   RAM.ORIGIN: starting address of RAM bank 0
 *   RAM.LENGTH: length of RAM bank 0
 */
MEMORY
{
  VECTORS (rx) : ORIGIN = 0x0, LENGTH = 0xC0
  FLASHCONF (rx) : ORIGIN = 0x400, LENGTH = 0x0F
  FLASH (rx) : ORIGIN = 0x410, LENGTH = 0x20000 - 0x410 /* 128K */
  RAM (rwx) : ORIGIN = 0x1ffff000, LENGTH = 16K /* 16K */
}
