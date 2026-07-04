ENTRY(Reset_Handler)

MEMORY
{
  FLASH   (rx)    : ORIGIN = 0x08000000,   LENGTH = 512K
  RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 128K
}

SECTIONS
{
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
    _sdata = .;
    *(.data)
    *(.data*)
    _edata = .;
  } >RAM AT> FLASH

  .bss :
  {
    _sbss = .;
    *(.bss)
    *(.bss*)
    _ebss = .;
    _end = .;
  } >RAM

  _sidata = LOADADDR(.data);
  _estack = ORIGIN(RAM) + LENGTH(RAM);

  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }
}
