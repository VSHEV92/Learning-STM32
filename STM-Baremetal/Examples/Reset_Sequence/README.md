# Reset Sequence Example

------

### Abstract

This example show what happens with processor under Reset Handler. This example show the most simple Vector Table, default values for Stack Pointer, Mode and Execution Level. Example ends by infinite loop, that implement simple counter

------

### How to Run

```
 make example=Reset_Sequence
```

------

### Example Description

This example implements the most simple **ISR Vector Table**:

```
.section .isr_vector, "a"
    .word  _end_of_stack   // Main Stack Pointer (MSP)
    .word  Reset_Handler   // Reset Vector Handler
```

Section property "a" needs to linker, so it knows that section is allocatable. Without this flag linker through this section away.

First entry is Main Stack Pointer (**MSP**). This value store in MSP after Reset. SP register points to MSP.

Second entry is Reset Handler. This is address of first instruction that will be executed after Reset. Reset is an Exception in ARM Architecture, so on reset entry processor perform interworking. ARMv6M support only Thumb state so all address in vector table need to has lowest bit set to 1. This can by done by linker, For that we need to point to linker that **Reset_Handler** label is function by following directive:

```
.type  Reset_Handler, %function
```

In this example Reset Handler just jump to **main** label.

After run GDB opens and halt processor on first instruction. All special registers will be moved to R0 register for illustration.  R0 register will be displayed after each Step.

1.  Before Reset R0 has some UNKNOWEN value. Press Step (s).
2. In STM32F0 core after reset R0 value is ones. Press Step (s).
3. We read **IPSR** register to R0. IPSR contaion Exception number of currently processed handler. The value of R0 is zero mean that no handler is processed and processor in **Thread mode**. Press Step (s).
4.  We read **CONTROL** register to R0. It's value also zero, CONTROL has to flags. **nPRIV** show Execution Level. Cortex-M0 support only privileged mode, so this bit is zero. Second bit is **SPSEL**, that show which SP bank is used. This bit is zero so we use MSP.  Press Step (s).
5. We read value of **MSP** to R0. It's value is 0x20001800. This is top address of RAM in STM32F042k6. This value is equal to value of first ISR Vector table entry. Press Step (s).
6. Next the loop is started. Press Step many times and we see that R0 register counts down from 10 to 0. 