# Reset Sequence Example

------

### Abstract

This example show what happens with processor under Reset Handler. This example show default values for Stack Pointer, Mode and Execution Level. Example ends by infinite loop, that implement simple counter

------

### How to Run

```
 make example=CMSIS/Reset_Sequence
```

------

### Example Description

After run GDB opens and halt processor on first instruction.

1.  Break point is set to first **main()** instruction. So you can bypass startup by pressing Continue (c).
3. We read **IPSR** register to **val** variable. **IPSR** contain Exception number of currently processed handler. Press Next (**n)** to execute one source code line. Then press **p val** to print **val** value. The value of **val** is zero mean that no handler is processed and processor in **Thread mode**. 
4.  We read **CONTROL** register to **val** variable. Press Next (**n)** and then print **val**. **CONTROL** register has two flags.  **nPRIV** show Execution Level. Cortex-M0 support only privileged mode, so this bit is zero. Second bit is **SPSEL**, that show which SP bank is used. 
6. Next the loop is started. Press Next many times and we that **val** variable is incremented. 
