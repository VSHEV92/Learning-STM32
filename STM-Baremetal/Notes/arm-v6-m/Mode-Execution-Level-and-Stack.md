# Mode, Execution Level and Stack

------

## Execution Level 

An M-profile processor supports two Execution Levels: **Privileged** and **Unprivileged**. Unprivileged level is optional and implemented under **Unprivileged/Privileged Extension**.

- **Privileged Level** - Application has access to all resources. Normally OS run under Privileged Level. Also faults and interrupts handled under this level.
- **Unprivileged Level** - Application has limited access to some resources. If implemented application normally run at this level. In unprivileged level there is no access to:
  - No access to **PRIMASK** and **CONTROL** registers.
  - Access to only **APSR** (N, Z, V, C) flags in **xPSR** register.
  - No access to **Private Peripheral Bus (PPB)**  memory space.
  - Instructions **CPSIE** and **CPSID** not supported. Instructions converted to **NOP**.
  - Instructions **MSR** and **MRS** not supported.

In Thread mode processor can run under unprivileged level. Bit **CONTROL.nPRIV** show if processor under privileged (0) or unprivileged (1) level.

Cortex-M0 does not implement unprivileged level.

After reset processor is running under privileged level.

------

## Mode

An M-profile processor supports two operating modes:

- **Thread mode** - Is entered on Reset, and can be entered as a result of an exception return. Application and OS code normally run under this mode.
- **Handler mode** - Is entered as a result of an exception. Processor gets inside this mode under exception handling.  

Processor mode can be found in **IPSR** register. In Thread mode mode **IPSR** is zero. In Handler mode mode **IPSR** contain Current Exception Number. 

Also processor mode can be found in System Interrupt Control and Status  (**ICSR**) register. In Thread mode mode **ICSR.VECTACTIVE** is zero. In Handler mode mode **ICSR.VECTACTIVE** contain Current Exception Number. 

In Thread mode processor can run under unprivileged or privileged level. In Handler mode processor always run under privileged level.

In Thread mode processor can use either Main Stack Pointer (**MSP**) or Process Stack Pointer (**PSP**). In Handler mode processor always use Main Stack Pointer (**MSP**).

After reset processor run under Thread mode.  

------

## Stack Pointer

The processor implements a banked pair of stack pointers, the Main Stack Pointer (**MSP** or **SP_main**) and the Process Stack Pointer (**PSP** or **SP_process**). 

In Thread mode processor can use either Main Stack Pointer (**MSP**) or Process Stack Pointer (**PSP**).  In this mode stack pointer can be controlled by **CONTROL.SPSEL** bit. If **CONTROL.SPSEL** is 0, then Main Stack Pointer is used. If **CONTROL.SPSEL** is 0, then Process Stack Pointer is used.

In Handler mode processor always use Main Stack Pointer (**MSP**). 

On Exception Entry hardware store context to current stack pointer, and then switch to  Main Stack Pointer. On Exception Return hardware switch stack pointer using **EXC_RETURN** value, and then restore context.

After reset processor use Main Stack Pointer.  

