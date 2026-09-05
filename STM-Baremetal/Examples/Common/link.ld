ENTRY(Reset_Handler)

MEMORY
{
    RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 6K
    FLASH   (rx)    : ORIGIN = 0x8000000,    LENGTH = 32K
}

SECTIONS
{
    
    _end_of_stack = ORIGIN(RAM) + LENGTH(RAM);

    .isr_vector :
    {
      KEEP(*(.isr_vector))
    } >FLASH
  
    .text :
    {
      *(.text)
      *(.text*)
    } >FLASH
  
    .rodata :
    {
      *(.rodata)
      *(.rodata*)
    } >FLASH
  
  
    .data :
    {
      _start_of_data = .;
      *(.data)
      *(.data*)
      _end_of_data = .;
    } >RAM AT> FLASH
  
    .bss :
    {
      _start_of_bss = .;
      *(.bss)
      *(.bss*)
      _end_of_bss = .;
    } >RAM


}

