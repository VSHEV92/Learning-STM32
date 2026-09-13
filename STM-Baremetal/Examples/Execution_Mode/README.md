# Execution Mode Example

------

### Abstract

This example show what is Thread and Handler Modes. **Thread Mode** is normal execution mode of processor. It runs in this mode after reset. Processor switches to **Handler Mode** when it trap to exception handler. 

------

### How to Run

```
 make example=Execution_Mode
```

------

### Example Description

This example implements to ISR Vector Handlers: Reset, HardFault, SVC and PendSV. Reset Handler just jump to **main** label. Other handler cause other nested exception and check processor mode by reading **IPSR** register.

First we trap to **PendSV** exception by setting it's pending bit. Inside it's handler we call **SVC** instruction to trap to **SVC** handler. Inside next handler we call **UDF** (undefined) instruction to cause **HardFault**.

After run GDB opens and halt processor on first instruction. All special registers will be moved to R0 register for illustration. All memory address are stores in R1 register. R0 and R1 register will be displayed after each Step.

**Configure Exceptions.**

1. Before Reset R0 has some UNKNOWEN value. In STM32F0 core after reset R0 value is ones. Press Step (s).
2. First of all we check that after Reset we in Thread mode. We read **IPSR** register to R0. R0 value is zero, so we in Thread mode. Press Step (s).
3. Next we setup interrupt priorities. We set **PendSV** priority to lower value, and **SVC** priority highest. So that SVC **can** preempt **PendSV** handler. We set R0 to 0x3 - lowest possible priority in Cortex-M0. Press Step (s).
4. We shift R0 by 22 bits. This is **PendSV** offset inside **SHPR3** register. Press Step (s).
5. We load **SHPR3** register address to R1 register. Press Step (s).
6. We store R0 register to address inside R1 register. So that we configure **PendSV** priority to 0x3.  Press Step (s).
7. Now we start configure **SVC**. We want to set it to lowet value - zero. So set R0 to 0. Press Step (s).
8. We load **SHPR2** register address to R1 register. Press Step (s).
9. We store R0 register to address inside R1 register. So that we configure **SVC** priority to 0x0.  Press Step (s).
10. Next we want to trigger **PendSV** exception. For that we need to assert it's pending bit. We store value 1 ro R0. Press Step (s).
11. We shift R0 by 28 bits. This is **PendSV** pending bit offset inside **ICSR** register. Press Step (s).
12. We load **ICSR** register address to R1 register. Press Step (s).
13. We store R0 register to address inside R1 register. So that we assert **PendSV** interrupt.  In Debug Mode we don't trap to this interrupt, so we start processor execution using Continue command. Processor trap to **PendSV** handler and stop at it's first instruction **BKPT** (Breakpoint). Press Continue (c).

**Inside Exceptions.**

1. Now we inside **PendSV** handler. Press Step (s).
2.  We read **IPSR** value to R0. Non-zero value means that we in **Handler Mode**. R0 is **0xe** or **14** in decimal. This is equal to number of **PendSV** exception. Press Step (s).
3. Next we call **SVC** instruction to trap to **SVC** handler. Press Step (s).
4.  We inside **SVC** handler. We read **IPSR** value to R0. R0 is **0xb** or **11** in decimal. This is equal to number of **SVC** exception. So we really preempt **PendSV** handler and trap to **SVC** handler. Press Step (s).
5. We execute **UDF** (undefined) instruction to trap **HardFault** handler. Press Step (s).
6. We inside **HardFault** handler. We read **IPSR** value to R0. R0 is **0x3**. This is equal to number of **HarFault** exception. We can not just return from this handler because it's return address points to **UDF** instruction. To continue program execution we need to change return address. Press Step (s).
7. We read Main Stack Point (**MSP**) address to R1. Press Step (s).
8. We add to R1 value **0x18**. This is offset of **Return Address** value from top of stack. So now R1 is pointing to **Return Address** value inside stack. Press Step (s).
9. We load **Return Address** value to register R0. Press Step (s).
10. We increment R0 by 2. So Now R0 points to instruction right after **UDF**. Press Step (s).
11. We store value of R0 to address inside R1 register. So that we update **Return Address** inside stack. Press Step (s).
12. We return from **HardFault** handler by BX instruction. Press Step (s).
13. Now again in **SVC** handler. We read **IPSR** value to R0. R0 is **0xb** or **11** in decimal. So we really return to **SVC** handler. Press Step (s).
14. We return from **SVC** handler by BX instruction. Press Step (s).
15. Now again in **PendSV** handler. We read **IPSR** value to R0. R0 is **0xe** or **14** in decimal. So we really return to **PendSV** handler. Press Step (s).
16. We return from **PendSV** handler by BX instruction. Press Step (s).
17. Now we return from all handler. Inside busy loop we constantly read **IPSR** value to R0 register. R0 is zero so we return to **Thread Mode**.