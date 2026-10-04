# NVIC Example

------

### Abstract

This example show features of Nested Vector Interrupt Controller (**NVIC**). We will configure three interrupts with different priorities. We set and clear enable and pending bits and check behavior of **PRIMASK** register. 

------

### How to Run

```
 make example=CMSIS-Core/NVIC
```

------

### Example Description

This example implements four ISR Vector Handlers: Reset and three IRQ Handlers. Reset Handler just jump to **main** label. Each IRQ has different priorities to check exception execution order rules.

After run GDB opens and halt processor on first instruction.

1.  Break point is set to first **main()** instruction. So you can bypass startup by pressing Continue (c).
2.  We configure interrupt priorities in following way: **IRQ0** - 0, **IRQ1** - 1, **IRQ2** - 2. We use **NVIC_SetPriority** function. Press Next (**n**) three times.
3.  We will check how **NVIC** enable bits is working. We set pending bit of **IRQ0** without enable. Interrupt will not triggered. We use **NVIC_SetPendingIRQ** function. Press Continue (c) to run processor. Processor will Halt on Break Point 3. 
4.  Now we set **IRQ0** enable bit. We use **NVIC_EnableIRQ** function. Press Continue (c) to run processor. Processor will Halt on Break Point 0. 
5.  We read **ICSR** to variable **val** check active exception. Press Next (**n**). We get value **0x10**, equal to **16** in decimal. Then we return from handler Press Next (**n**).
6.  We check how **PRIMASK** register works. We disable all interrupts using **__disable_irq()** function. And then we set pending bit of IRQ0. Interrupt will not triggered. Press Continue (c) to run processor. Processor will Halt on Break Point 4. 
7.  Now we set enable interrupts using **__enable_irq()** function. Press Continue (c) to run processor. Processor will Halt on Break Point 0. This is exception handler of **IRQ0**. 
8.  We read **ICSR** to variable **val** check active exception. Press Next (**n**). We get value **0x10**, equal to **16** in decimal. Then we return from handler Press Next (**n**).
9.  Now we trigger all interrupts. First we disable **IRQ0** using **NVIC_DisableIRQ()** function, Then we set pending bits for all three interrupts using **NVIC_SetPendingIRQ** function. Press Next (**n**) four times.
10.  We enable all interrupts at once by direct writing to **NVIC** **ISER** register. Press Continue (c) to run processor. 
11.  Processor will Halt on Break Point 0. **IRQ0** handled first because it is highest priority interrupt. Press Continue (c) to run processor. 
12.  Processor will Halt on Break Point 1. **IRQ1** handled next because has next highest priority interrupt. We clear pending bit of **IRQ2** using **NVIC_ClearPendingIRQ()** function. So that this interrupt will not triggered. Press Continue (c) to run processor. 
13.  Processor will Halt on Break Point 5. Processor will not halt on Break Point 2 because we clear pending bit. 

