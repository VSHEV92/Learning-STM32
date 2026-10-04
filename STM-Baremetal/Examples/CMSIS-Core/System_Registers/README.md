# System Register Example

------

### Abstract

This example show features of almost all Cortex-M0 system register.

------

### How to Run

```
make example=CMSIS-Core/System_Register
```

------

### Example Description

This example implements to ISR Vector Handlers: Reset, NMI, SysTick and PendSV. Reset Handler just jump to **main** label. NMI handler check **ICSR** and return. PendSV handler check ICSR and then clear pending for SysTick. SysTick handler never run.

After run GDB opens and halt processor on first instruction.

1. Break point is set to first **main()** instruction. So you can bypass startup by pressing Continue (c).

2. We read **CPUID** register. Press Next (n). We get value **0x410cc200**. This value contain following code:

   - Implementer: 0x41 = ARM
   - Variant: 0x0 = major revision number
   - Constant: 0xC = ARMv6-M architecture,
   - Partno: 0xC20 = Cortex-M0
   - Revision: 0x0 = minor revision number 

3. Next read **AIRCR**. Press Next (n). We get value **0xfa050000**. 

   - Highest 16 bits is **VECTKEYSTAT**, this value is **UNKNOWN** in  ARMv6-M architecture. In STM32F0 this value is byte revers of **VECTKEY** equal to **0x05FA**.
   - Bit 15 is zero. This means that core run in little endian format. 

4. Next read **CCR**. Press Next (n). We get value **0x208**.

   - Bit 9 is set - **STKALIGN**. Processor align stack to 8 bytes on exception entry
   - Bit 3 is set - **UNALIGN_TRP**. Processor trap to HardFault on unaligned memory access.

5. Next we set SysTick interrupt priority to lowest value 0b11 using **NVIC_SetPriority()** function. Press Next (n). 

6. We assert **NMI**, **PendSV** and **SysTick** exceptions using pending bits in **ICSR**.  Press Continue (c).

7. Now we in NMI handler. We will exam **ICSR**.  Press Next (n). We store **ICSR** to **val** variable. We get value **0x14000002**. We see following:

   - Bit 31 is clear. This is **NMI** pending bit. Processor clear this bit automatically on exception entry.
   - Bit 28 is set. **PendSV** exception steel pending. 
   - Bit 26 is set. **SysTick** exception steel pending. 
   - **VECTACTIVE** is equal to **0x2**. This corresponds to **NMI** exception.
   - **VECTPENDING** is zero. But **PendSV** and **SysTick** are pending. This is strange.

8. Press Continue (c) to exit from handler.

9. Now we in **PendSV** handler, because is has higher priority then **SysTick**. We will exam **ICSR**. Press Next (n). We store **ICSR** to **val** variable. We get value **0x400000e**. We see following:

   - Bit 28 is clear. This is **PendSV** pending bit. Processor clear this bit automatically on exception entry.

   - Bit 26 is set. **SysTick** exception steel pending. 
   - **VECTACTIVE** is equal to **0xe**, which is **14** in decimal. This corresponds to **PendSV** exception.

10. Next we clear **SysTick** pending bit and check **ICSR** value again. Press Next (n) two times. We get value **0xe**. We see following:

    - Bit 26 is clear. **SysTick** exception is not pending now. 
    - **VECTACTIVE** is equal to **0xe**, which is **14** in decimal. This corresponds to **PendSV** exception.

11. Press Continue (c) to exit from handler. Now we in finish busy loop. We will not jump to **SysTick** handler, because we clear it's pending bit.

