# Exception Model Example

------

### Abstract

This example show how exception entry and return work in ARM processors. We look at stack under exception trap from Thread and Handler modes.  

------

### How to Run

```
 make example=Assembly/Exception_Model
```

------

### Example Description

This example implements to ISR Vector Handlers: Reset, SVC and PendSV. Reset Handler just jump to **main** label. Other handler cause other nested exception and check context in stack.

Thread mode use **PSP**, set exception priorities, initialize register with some values and set **PendSV**.

First we trap to **PendSV** exception by setting it's pending bit. Inside it's handler we call **SVC** instruction to trap to **SVC** handler. 

After run GDB opens and halt processor on first instruction. All special registers will be moved to R0 register for illustration. All memory address are stores in R1 register. R0 and R1 register will be displayed after each Step.

**Configure Exceptions and Stack Configuration.**

1. Before Reset R0 has some UNKNOWEN value. In STM32F0 core after reset R0 value is ones. Press Step (s).
2. First we setup interrupt priorities. We set **PendSV** priority to lower value, and **SVC** priority highest. So that SVC **can** preempt **PendSV** handler. We set R0 to 0x3 - lowest possible priority in Cortex-M0. Press Step (s).
3. We shift R0 by 22 bits. This is **PendSV** offset inside **SHPR3** register. Press Step (s).
4. We load **SHPR3** register address to R1 register. Press Step (s).
5. We store R0 register to address inside R1 register. So that we configure **PendSV** priority to 0x3.  Press Step (s).
6. Now we start configure **SVC**. We want to set it to lowet value - zero. So set R0 to 0. Press Step (s).
7. We load **SHPR2** register address to R1 register. Press Step (s).
8. We store R0 register to address inside R1 register. So that we configure **SVC** priority to 0x0.  Press Step (s).
9. Next we switch process to **PSP**. We set Process Stack Pointer top 1 KB lower then RAM top address. For that we load top RAM address to R1, Then we load value 1024 to R0 and subtract R0 from R1. Now R1 is equal to **0x20001400**.  We move this value to **PSP** using **MSR** instruction,  Press Step (s) four times.
10. Next we switch to **PSP**, For that we need set **bit[1]** in **CONTROL** register. We load value 2 to R0, to set first bit. We move this value to **CONTROL** register using **MSR** instruction. Then we use **ISB** instruction to flush processor pipeline. Press Step (s) three times.
11. We initialize registers R0 to R3 and R12 to values from 1 to 5. This values will be moved to stack on exception entry. Press Step (s) six times.
12. Next we want to trigger **PendSV** exception. For that we need to assert it's pending bit. We store value 1 ro R4. Press Step (s).
13. We shift R4 by 28 bits. This is **PendSV** pending bit offset inside **ICSR** register. Press Step (s).
14. We load **ICSR** register address to R5 register. Press Step (s).
15. We store R4 register to address inside R5 register. So that we assert **PendSV** interrupt.  In Debug Mode we don't trap to this interrupt, so we start processor execution using Continue command. Processor trap to **PendSV** handler and stop at it's first instruction **BKPT** (Breakpoint). Press Continue (c).

**Inside Exceptions.**

1. Now we inside **PendSV** handler. Press Step (s).
2. We want to inspect process stack. Before trap we use **PSP** as stack pointer. So we load **PSP** value to R1 register. **PSP** value is **0x200013e0**. This is 8 word lower then previous value **0x20001400**.  Press Step (s).
3. We start reading stack values one by one. First five values (offsets **0** to **16**) is register R0-R3 and R12. Stack contain values that we store in these registers before. Press Step (s) five times.
4. In offset **20** stack contain **LR** value. We not initialize this register before, so we get reset value with all ones. Press Step (s).
5. In offset **24** stack contain **Return Address**. We get **0x80000c6**. This is the address **NOP** instruction in finish busy loop. Press Step (s).
6. In offset **28** stack contain value of **xPSR**. We get value **0x01000000**. Only bit 24 is set. This is **T** bit, that indicate that processor in Thumb state. Press Step (s).
7. Next we initialize R0 to R3 and R12 to values from 0xF1 to 0xF5. This values will be moved to stack on next exception entry. Press Step (s) six times.
8. Next we call **SVC** instruction to trap to **SVC** handler. Press Step (s).
9. Now we again want to inspect process stack. Before trap we was in Handler mode, so we use **MSP** as stack pointer. So we load **MSP** value to R1 register. **MSP** value is **0x200017e0**. This is 8 word lower then previous value **0x20001800** - top of RAM.  Press Step (s).
10. We start reading stack values one by one. First five values (offsets **0** to **16**) is register R0-R3 and R12. Stack contain values that we store in these registers before. Press Step (s) five times.
11. In offset **20** stack contain **LR** value. This value is equal to **0xfffffffd**. This is **Exception Return** of **PendSV** handler. We will look at this value later. Press Step (s).
12. In offset **24** stack contain **Return Address**. We get **0x8000062**. This is the address of next instruction after **SVC**. Press Step (s). 
13. In offset **28** stack contain value of **xPSR**. We get value **0x100000e**. Bit 24 is set. This is **T** bit, that indicate that processor in Thumb state. Also lower 6 bits set to **0xe**, This in **14** in decimal - number of **PendSV** exception number. Press Step (s).
14. Next we load LR value to R0 to examine **Exception Return** value. We get **0xfffffff1**. Upper 4 bits is all ones, so this is a exception return value. Lower 4 bits is equal to **0x1**. This mean we will back to **Handler Mode**. Exception return gets state from the **Main stack**. On return execution uses the **Main Stack**. Press Step (s).
15. We return from **SVC** handler by BX instruction. Press Step (s).
16. Now again in **PendSV** handler. we load LR value to R0 to examine **Exception Return** value. We get **0xfffffffd**. Upper 4 bits is all ones, so this is a exception return value. Lower 4 bits is equal to **0xd**. This mean we will back to **Thread Mode**. Exception return gets state from the **Process stack**. On return execution uses the **Process Stack**. Press Step (s).
17. We return from **PendSV** handler by BX instruction. Press Step (s).
