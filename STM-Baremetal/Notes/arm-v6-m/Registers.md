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

### Special Purpose Registers

#### Program Status Registers (xPSR)

The Program Status Register (**PSR**) is a 32-bit register that comprises three subregisters:

- **Application Program Status Register (APSR)** - Holds flags that can be written by application-level software, that is, by unprivileged software. This register contain 4 bits:
  - **N, bit [31]** - Negative condition code flag. If the result is regarded as a two's complement signed integer, then N is set to 1 if the result is negative and set to 0 if it is positive or zero.
  - **Z, bit [30]** - Zero condition code flag. Set to 1 if the result of the instruction is zero, and to 0 otherwise. A result of zero often indicates an equal result from a comparison.
  - **C, bit [29]** - Carry condition code flag. Set to 1 if the instruction results in a carry condition, for example an unsigned overflow on an addition.
  - **V, bit [28]** - Overflow condition code flag. Set to 1 if the instruction results in an overflow condition, for example a signed overflow on an addition.
- **Interrupt Program Status Register (IPSR)** - When the processor is executing an exception handler, holds the exception number of the exception being processed. Otherwise, the **IPSR** value is zero. In Thread mode, the value is 0, in Handler mode, holds the exception number of the currently-executing exception. On reset, the processor is in Thread mode and the Exception Number field of the IPSR is set to 0.
- **Execution Program Status Register (EPSR)** - Holds Execution state bits. The **EPSR** contains the **T**-bit, that indicates whether the processor is in Thumb state. The **T**-bit cannot be read by software. All fields Read-As-Zero using an **MRS** instruction, and the processor ignores writes to the **EPSR** by an **MSR** instruction. A reset sets the **T** bit to the value of bit [0] of the reset vector.



#### Mask Register (PRIMASK)

The processor can use the exception mask register **PRIMASK**, that is used for priority boosting. **PRIMASK** is a special-purpose mask register. **PRIMASK.PM** is set to 0 on reset. Software can access this register using the **MSR** or **MRS** instructions. In addition: 

- Executing the instruction **CPSID i** sets **PRIMASK.PM** to 1.
- Executing the instruction **CPSIE i** sets **PRIMASK.PM** to 0.



#### CONTROL Register

The special-purpose **CONTROL** register is a 2-bit special-purpose register defined as follows:

- **nPRIV (bit[0])** - If the Unprivileged/Privileged Extension is implemented, defines the execution privilege in Thread mode:
  - 0 - Thread mode has privileged access.
  - 1 - Thread mode has unprivileged access.
- **SPSEL (bit[1])** - Defines the stack to be used:
  - 0 - Use SP_main as the current stack.
  - 1 - In Thread mode, use SP_process as the current stack. In Handler mode, this value is reserved.

Software can update the SPSEL bit in privileged Thread mode. In Handler mode, the processor ignores explicit writes to the SPSEL bit.

**Software must use an ISB barrier instruction to ensure a write to the CONTROL register takes effect before the next instruction is executed.**

