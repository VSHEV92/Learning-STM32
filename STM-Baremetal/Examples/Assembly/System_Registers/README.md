# System Register Example

------

### Abstract

This example show features of almost all Cortex-M0 system register.

------

### How to Run

```
 make example=Assembly/System_Register
```

------

### Example Description

This example implements to ISR Vector Handlers: Reset, NMI, SysTick and PendSV. Reset Handler just jump to **main** label. NMI handler check **ICSR** and return. PendSV handler check ICSR and then clear pending for SysTick. SysTick handler never run.

After run GDB opens and halt processor on first instruction. All special registers will be moved to R0 register for illustration. All memory address are stores in R1 register. R0 and R1 register will be displayed after each Step.

**Read Only Features.**

1. Before Reset R0 has some UNKNOWEN value. In STM32F0 core after reset R0 value is ones. We will read some system register and exam their feilds. Press Step (s).

2. First read **CPUID**. Press Step (s) two times. In R0 we get value **0x410cc200**. This value contain following code:

   - Implementer: 0x41 = ARM
   - Variant: 0x0 = major revision number
   - Constant: 0xC = ARMv6-M architecture,
   - Partno: 0xC20 = Cortex-M0
   - Revision: 0x0 = minor revision number 

3. Next read **VTOR**. Press Step (s) two times. In R0 we get value **0x0**. In Cortex-M0 vector table is always at **0x0** address and can not be reloacated.

4. Next read **AIRCR**. Press Step (s) two times. In R0 we get value **0xfa050000**. 

   - Highest 16 bits is **VECTKEYSTAT**, this value is **UNKNOWN** in  ARMv6-M architecture. In STM32F0 this value is byte revers of **VECTKEY** equal to **0x05FA**.
   - Bit 15 is zero. This means that core run in little endian format. 

5. Next read **CCR**. Press Step (s) two times. In R0 we get value **0x208**.

   - Bit 9 is set - **STKALIGN**. Processor align stack to 8 bytes on exception entry
   - Bit 3 is set - **UNALIGN_TRP**. Processor trap to HardFault on unaligned memory access.

   

**Configure and assert exceptions.**

1. Set **SysTick** exception priority to lowest value 0x3. We use **SHPR3** to do that. Press Step (s) four times.
2. Assert **NMI**, **PendSV** and **SysTick** exceptions using pending bits in **ICSR**. Press Step (s) 12 times.
3. At the and call **DSB** instruction to wait for **SCS** registers update. Press Step (s).

**Inside exceptions.**

1. Now we in NMI handler. We will exam **ICSR**. Press Step (s) two times. In R0 we get value **0x14000002**. We see following:

   - Bit 31 is clear. This is **NMI** pending bit. Processor clear this bit automatically on exception entry.
   - Bit 28 is set. **PendSV** exception steel pending. 
   - Bit 26 is set. **SysTick** exception steel pending. 
   - **VECTACTIVE** is equal to **0x2**. This corresponds to **NMI** exception.
   - **VECTPENDING** is zero. But **PendSV** and **SysTick** are pending. This is strange.

   Press Continue (c) to jump to next handler.

2. Now we in **PendSV** handler, because is has higher priority then **SysTick**. We will exam **ICSR**. Press Step (s) two times. In R0 we get value **0x400000e**. We see following:

   - Bit 28 is clear. This is **PendSV** pending bit. Processor clear this bit automatically on exception entry.
   - Bit 26 is set. **SysTick** exception steel pending. 
   - **VECTACTIVE** is equal to **0xe**, which is **14** in decimal. This corresponds to **PendSV** exception.

3. Next we clear **SysTick** pending bit and check **ICSR** value again. Press Step (s) five times. In R0 we get value **0xe**. We see following:

   - Bit 26 is clear. **SysTick** exception is not pending now. 
   - **VECTACTIVE** is equal to **0xe**, which is **14** in decimal. This corresponds to **PendSV** exception.

   Press Step (s) to return from handler.

4. Now we in finish busy loop. We will not jump to **SysTick** hanlder, because we clear it's pending bit.



 
