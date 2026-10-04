# Stack Alignment Example

------

### Abstract

This example show how processor perform stack alignment on exception entry. The AAPCS requires that the stack-pointer be 8-byte aligned on entry to a conforming function. However, because exceptions can occur on any instruction boundary, it is possible that the current stack pointer is not 8-byte aligned when an exception activates. Processor automatically align stack on exception entry.

------

### How to Run

```
make example=CMSIS-Core/Stack_Alignment
```

------

### Example Description

This example implements to ISR Vector Handlers: Reset and SVC. Reset Handler just jump to **main** label. SVC handler inspect stack pointer value and alignment bit in **xPSR**.

After run GDB opens and halt processor on first instruction.

1.  Break point is set to first **main()** instruction. So you can bypass startup by pressing Continue (c).
2.  We read **MSP** register to **val** variable. It's value is **0x200017e8**. This is 24 bytes below address of top of RAM. Stack pointer was modified by 24 bytes on main function entry. Lower three bits of address is zeros, so **MSP** is 8 bytes aligned. Press Next (**n)**. 
3.  We call **SVC** to trap to handler. Press Next (**n)**. 
4.  We read **MSP** register to **val** variable. It's value is **0x200017c8**. This value is 32 bytes below previous value because 8 words was stored on exception entry. **SVC** handler has attribute **naked**, so no other modification on stack was performed. Press Next (**n)**. 
5.  On offset **28** bytes from top of stack processor store **xPSR** value. We read it to **val** variable. e get  **0x61000000**. Bits 9 is zero so processor not perform stack alignment. Press Next (**n)**. 
6.  We leave handler using **BX** instruction. Press Next (**n)**. 
7.  We read **MSP** register to **val** variable. Now it's value again  **0x200017e8**. Press Next (**n)**. 
8.  We push on word to stack. Press Next (**n)**. 
9.  We read **MSP** register to **val** variable. It's value is **0x200017e4**. This value is 4 bytes (1 word) lower than previous value. Only to lower bits are zeros. So stack pointer is only 4 bytes aligned. Press Next (**n)**. 
10.  We call **SVC** to trap to handler. Press Next (**n)**. 
11.  We read **MSP** register to **val** variable. It's value is **0x200017c0**. This value is 36 bytes below previous value. 32 bytes came from 8 words, stored on stack on exception entry. 4 bytes because processor align stack pointer to 8 bytes. Press Next (**n)**. 
12.  On offset **28** bytes from top of stack processor store **xPSR** value. We read it to **val** variable. e get  **0x61000200**. its 9 is one so processor perform stack alignment. Processor use this bit to restore stack value on exception return. Press Next (**n)**. 
13.  We leave handler using **BX** instruction. Press Next (**n)**. 
14.  We read **MSP** register to **val** variable. Now it's value again  **0x200017e4**. Press Next (**n)**. 

 
