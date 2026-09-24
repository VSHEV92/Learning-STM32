# SysTick Example

------

### Abstract

This example show features System Timer (**SysTick**) and Wait for Interrupt (**WFI**) instruction.

------

### How to Run

```
 make example=Assembly/SysTick
```

------

### Example Description

This example implements two ISR Vector Handlers: Reset and SysTick. Reset Handler just jump to **main** label. SysTick **SYST_CSR** to check and clear counter flag. Design configure SysTick to fire interrupt every second, assuming that core clock frequency is 8 MHz. Also core is configured to enter sleep mode on exception return.

After run GDB opens and halt processor on first instruction. All special registers will be moved to R0 register for illustration. All memory address are stores in R1 register. R0 and R1 register will be displayed after each Step.

**Configure Core.**

1. Before Reset R0 has some UNKNOWEN value. In STM32F0 core after reset R0 value is ones. Press Step (s).

2. Configure core to enter Sleep Mode on exception return. For that we need to set **SLEEPONEXIT (bit[1])** in **SCR**. Press Step (s) three times.

3. Next we configure Reload Value of SysTick so that interrupt triggers every second. We need to store to SYST_RVR value equal to 8000000 - 1. Press Step (s) four times.

4. We will clear SysTick Current Value and Counter Flag to zero, For that will need to write any value to **SYST_CVR**. Then we load **SYST_CVR** to R0 register to check that it is zero. Press Step (s) three times.

5. We finish SysTick configuration and run it. In **SYST_CSR** we set to one following bits:

   - **CLKSOURCE** - set to one to select processor clock

   - **TICKINT** - set to one to enable SysTick interrupts

   - **ENABLE** - set to one to start SysTick

   Press Step (s) three times.

6. Then we call **DSB** instruction to wait for memory transaction completion and then go to Sleep using **WFI**. Press Continue (c) to exit from Debug State.

   

**SysTick Interrupts.**

1. After one second SysTick interrupt will fire. We will stop at first **BKPT** instruction of SysTick Exception handler. 

2. We check that Counter Flag is set. For That we need to read **SYS_CSR**.  Press Step (s) two times. In R0 register we get value **0x10007**. Bit **COUNTFLAG** (**bit[16]**) is set, which means counter reach zero value.

3.  Counter Flag is cleared on read. We read **SYS_CSR**. Press Step (s). Now in R0 register we get value **0x7**. So **COUNTFLAG** is clear now. Press Continue (c) to exit from Debug State. SysTick interrupt will fire again one second later.

   
