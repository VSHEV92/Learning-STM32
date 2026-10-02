ENTRY(Reset_Handler)

MEMORY
{
    RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 6K
    FLASH   (rx)    : ORIGIN = 0x8000000,    LENGTH = 32K
}

SECTIONS
{
    

    .isr_vector :
    {
      . = ALIGN(4);  
      KEEP(*(.isr_vector))
    } >FLASH
  
    .text :
    {
      . = ALIGN(4);  
      *(.text)
      *(.text*)
    } >FLASH
  
    .rodata :
    {
      . = ALIGN(4);  
      *(.rodata)
      *(.rodata*)
    } >FLASH
  
  
    .data :
    {
      . = ALIGN(4);  
      _start_of_data = .;
      *(.data)
      *(.data*)
      _end_of_data = .;
    } >RAM AT> FLASH
  
    .bss :
    {
      . = ALIGN(4);  
      _start_of_bss = .;
      *(.bss)
      *(.bss*)
      _end_of_bss = .;
    } >RAM

    _start_of_flash_data = LOADADDR(.data);
    _end_of_stack = ORIGIN(RAM) + LENGTH(RAM);

}

