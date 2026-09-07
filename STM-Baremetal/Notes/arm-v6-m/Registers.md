# Register

------

### General Purpose Registers

ARM Processor has 16 general purpose register. Register 0 to 12 are common register without any special features. Last three processor registers have special functions:

- **Stack Pointer (SP, R13)** -  used as a pointer to the active stack. This is a banked register that mapped to two other register Main Stack Pointer (**MSP**) and Process Stack Pointer (**PSP**). In Handler mode SP is always map to MSP. In Thread mode SP can map to MSP or PSP base on **CONTROL.SPSEL** bit.  Value of SP need to be **8-byte aligned**. MSP is used after Reset and contain zero entry of Vector table. 
- **Link Register (LR, R14)** - stores the Return Link. This is a value that relates to the return address from a subroutine that is entered using a Branch with Link instruction. This register updates by **BL** and **BLX** instructions. LR also updates under exception entry. It contain Exception Return value.
- **Program Counter (PC, R15)** - contain address of current instruction plus 4. The PC is loaded with the Reset handler start address on reset. PC update by Branch instruction and also by Store and Arithmetic instruction when it used as destination register. Bit zero not used in address calculations for this instruction. In some cases bit number zero determine next processor state: ARM or Thumb. This feature is named interworking. ARMv6M support only Thumb state so for this instruction lowest bit need to has value 1. Following instruction cause interworking:
  - BLX, BX - branch with interworking
  - All store instruction to PC (STR, STRH, STRB, PUSH)
  - Exception entry. Entry value need has lowest bit set to 1   

------

