# System Control Space

------

## Address Map

The address space **0xE0000000** to **0xFFFFFFFF** is reserved for **system level** use.



The **System region** of the memory map, starting at **0xE0000000**, subdivides as follows:

- the 1MB region at offset **0x00000000** is reserved as a Private Peripheral Bus (**PPB**).
- the region from offset **0x00100000** is the Vendor system region, **Vendor_SYS**.



The general rules for the **PPB**, address range **0xE0000000** to **0xE0100000**, are:

- The region is defined as Strongly-ordered memory
- Registers accesses are always accessed little endian regardless of the endian state of the processor
- The PPB address space only supports aligned word accesses
- Unprivileged accesses to the PPB generate a HardFault error, without causing a PPB access



The **System Control Space (SCS)** is a memory-mapped 4KB address space that provides 32-bit registers for configuration, status reporting and control. The **SCS** registers divide into the following groups:

- system control and identification.
- the CPUID processor identification space.
- system configuration and status (**System Control Block**).
- an optional system timer, **SysTick**.
- a Nested Vectored Interrupt Controller (**NVIC**).
- system debug



------

## System Control Block

In an ARMv6-M a **System Control Block (SCB)** in the SCS provides key status information and control features for the processor. The SCB supports following register:



#### CPUID Register (0xE000ED00)

The CPUID Base Register provides identification information for the processor:

- IMPLEMENTER
- VARIANT
- ARCHITECTURE
- PARTNO
- REVISION



#### Interrupt Control State Register, ICSR (0xE000ED04)

Controls and provides status information for the ARMv6-M:

- Pending set bits for NMI, PendSV and SysTick
- Pending clear bits for PendSV and SysTick
- **ISRPREEMPT** - Indicates whether a pending exception will be serviced on exit from debug
- **ISRPENDING** - Indicates if an external configurable, NVIC generated, interrupt is pending
- The exception number for the highest priority pending exception (**VECTPENDING**)
- The exception number for the current executing exception (**VECTACTIVE**)



#### Vector Table Offset Register, VTOR (0xE000ED08)

Holds the vector table offset address. For ARMv6-M all bits are zeros.



#### Application Interrupt and Reset Control Register, AIRCR (0xE000ED0C)

Sets or returns interrupt control data. Access to this register is block by **VECTKEY**. This value should be equal to **0x05FA**. Register has following features:

- **ENDIANNESS** - Indicates the memory system data endianness
- **SYSRESETREQ** - System Reset Request to external power controller. Software can reset core using this bit
- **VECTCLRACTIVE** - Clears all active state information for fixed and configurable exceptions. Used by debugger to return core to initial state. This bit claer all active flags for all exceptions  



#### System Control Register, SCR (0xE000ED10)

Configure processor power management features:

- interrupt transition from inactive state to pending state is a wakeup event (**SEVONPEND**)
- use deep sleep mode. indicating that waking from sleep might take longer (**SLEEPDEEP**)
- on an exit from an ISR that returns to the base level of execution priority, the processor enters a sleep state (**SLEEPONEXIT**)



#### Configuration and Control Register, CCR (0xE000ED14)

- Determine if stack alignment is using (**STKALIGN**). Always one 

- Is unaligned word and halfword accesses generate a HardFault exception (**UNALIGN_TRP**). Always one 

  

#### System Handler Priority Registers, SHPR2 (0xE000ED1C) and SHPR3 (0xE000ED20)

Determine exception priorities for system exceptions:

- Supervisor Call (**SVC**)
- **PendSV**
- **SysTick**



------

## Debug Registers