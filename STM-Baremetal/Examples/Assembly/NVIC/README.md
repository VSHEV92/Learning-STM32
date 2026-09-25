# NVIC Example

------

### Abstract

This example show features of Nested Vector Interrupt Controller (**NVIC**). We will configure three interrupts with different priorities. We set and clear enable and pending bits and check behavior of **PRIMASK** register. 

------

### How to Run

```
 make example=Assembly/NVIC
```

------

### Example Description

This example implements four ISR Vector Handlers: Reset and three IRQ Handlers. Reset Handler just jump to **main** label. Each IRQ has different priorities to check exception execution order rules.

After run GDB opens and halt processor on first instruction. All special registers will be moved to R0 register for illustration. All memory address are stores in R1 register. R0 and R1 register will be displayed after each Step.

**Configure Priorities.**

1. Before Reset R0 has some UNKNOWEN value. In STM32F0 core after reset R0 value is ones. Press Step (s).

2. We configure interrupt priorities in following way: **IRQ0** - 0, **IRQ1** - 1, **IRQ2** - 2. Press Step (s) six times.


**NVIC Enable Interrupts.**

1. We will check how **NVIC** enable bits is working. We set pending bit of **IRQ0** without enable. Interrupt will not triggered. Press Continue (c) to run processor. Processor will Halt on Break Point 3. 

2. Now we set **IRQ0** enable bit and call **WFI** instruction. Press Continue (c) to run processor. Processor will Halt on Break Point 0. This is exception handler of **IRQ0**. 

3. We read **ICSR** to R0 register check active exception. We get value **0x10**, equal to **16** in decimal. This is number of **IRQ0**. Press Step (s) three times.

**Global Enable Interrupts.**

1. We check how **PRIMASK** register works. We disable all interrupts using **CPSID** instruction. And then we set pending bit of IRQ0. Interrupt will not triggered. Press Continue (c) to run processor. Processor will Halt on Break Point 4. 
2. Now we set enable interrupts using **CPSIE** and call **WFI** instruction. Press Continue (c) to run processor. Processor will Halt on Break Point 0. This is exception handler of **IRQ0**. 
3. We read **ICSR** to R0 register check active exception. We get value **0x10**, equal to **16** in decimal. This is number of **IRQ0**. Press Step (s) three times.

**Trigger all interrupts.**

1. Now we trigger all interrupts. We enable all interrupts and set all pending bits. Press Continue (c) to run processor. 

2. Processor will Halt on Break Point 0. **IRQ0** handled first because it is highest priority interrupt. Press Continue (c) to run processor. 

3. Processor will Halt on Break Point 1. **IRQ1** handled next because has next highest priority interrupt.

4. We clear pending bit of **IRQ2**. So that this interrupt will not triggered. Press Continue (c) to run processor. 

5. Processor will Halt on Break Point 5. Processor will not halt on Break Point 2 because we clear pending bit.

   

   
