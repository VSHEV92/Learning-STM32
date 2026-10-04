# SysTick Example

------

### Abstract

This example show features System Timer (**SysTick**) and Wait for Interrupt (**WFI**) instruction.

------

### How to Run

```
make example=CMSIS-Core/SysTick
```

------

### Example Description

This example implements two ISR Vector Handlers: Reset and SysTick. Reset Handler just jump to **main** label. SysTick **SYST_CSR** to check and clear counter flag. Design configure SysTick to fire interrupt every second, assuming that core clock frequency is 8 MHz. Also core is configured to enter sleep mode on exception return.

After run GDB opens and halt processor on first instruction.

1. Break point is set to first **main()** instruction. So you can bypass startup by pressing Continue (c).

2. Configure core to enter Sleep Mode on exception return. For that we need to set **SLEEPONEXIT (bit[1])** in **SCR**. Press Next (n).

3. Next we configure Reload Value of SysTick so that interrupt triggers every second. We need to store to **SYST_RVR** value equal to 8000000 - 1. Press Next (n).

4. We will clear SysTick Current Value and Counter Flag to zero, For that will need to write any value to **SYST_CVR**. Press Next (n).

5. We finish SysTick configuration and run it. In **SYST_CSR** we set to one following bits:

   - **CLKSOURCE** - set to one to select processor clock

   - **TICKINT** - set to one to enable SysTick interrupts

   - **ENABLE** - set to one to start SysTick

   Press Next (n).

6. Then we call **DSB** instruction to wait for memory transaction completion and then go to Sleep using **WFI**. Press Continue (c) to exit from Debug State.

7. After one second SysTick interrupt will fire. We will stop at first **BKPT** instruction of SysTick Exception handler. 

8. We check that Counter Flag is set. For That we need to read **SYS_CSR**. Press Next (n). In **val** variable we get value **0x10007**. Bit **COUNTFLAG** (**bit[16]**) is set, which means counter reach zero value.

9. Counter Flag is cleared on read. We read **SYS_CSR**.  Press Next (n). Now nn **val** variable  we get value **0x7**. So **COUNTFLAG** is clear now. Press Continue (c) to exit from Debug State. SysTick interrupt will fire again one second later.
