# Lockup Example

------

### Abstract

This example show how Lockup behavior.  

------

### How to Run

```
 make example=CMSIS-Core/Lockup
```

------

### Example Description

This example implements two ISR Vector Handlers: Reset and HardFault. Reset Handler just jump to **main** label. HardFault call **UDF** instruction to cause **Lockup**.

After run GDB opens and halt processor on first instruction.

1.  Break point is set to first **main()** instruction. So you can bypass startup by pressing Continue (c).
2.  Next we call undefined instruction (**UDF**) using inline assembly to trap to **HardFault**. Press Next (**n**) .
3.  Now we inside HArdFault handler. We read **IPSR** register to **val** variable. This variable is **0x3**. This is equal to number of **HarFault** exception. Press Next (**n**) two times.
4.  Next again we call undefined instruction (**UDF**) using inline assembly. This time processor switch to **Lockup** state Press Next (**n**) .
5.  Now we in **Lockup State**. Process try to execute instruction from **0xFFFFFFFE** address. This address is marked as **XN**, resulting in a further Lockup instruction that keeps the processor in **Lockup state**.

