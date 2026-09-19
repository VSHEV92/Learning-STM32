# Stack Alignment Example

------

### Abstract

This example show how processor perform stack alignment on exception entry. The AAPCS requires that the stack-pointer be 8-byte aligned on entry to a conforming function. However, because exceptions can occur on any instruction boundary, it is possible that the current stack pointer is not 8-byte aligned when an exception activates. Processor automatically align stack on exception entry.

------

### How to Run

```
 make example=Assembly/Stack_Alignment
```

------

### Example Description

This example implements to ISR Vector Handlers: Reset and SVC. Reset Handler just jump to **main** label. SVC handler inspect stack pointer value and alignment bit in **xPSR**.

After run GDB opens and halt processor on first instruction. All special registers will be moved to R0 register for illustration.  R0 register will be displayed after each Step.

1. Before Reset R0 has some UNKNOWEN value. In STM32F0 core after reset R0 contain is ones. Press Step (s).
2. We read **MSP** to R0 register. **MSP** value is **0x20001800**. This value is equal to zero entry in ISR vector and is top address of RAM. Lower three bits of address is zeros, so MSP is 8 bytes aligned. Press Step (s).
3. We call **SVC** to trap to handler. Press Step (s).
4. On exception entry processor store context of 8 words to stack. We read **MSP** to R0 register. Now **MSP** value is **0x200017e0**. This 8 words lower that previous value. Press Step (s).
5. On offset **28** from top of stack processor store **xPSR** value. We load this value to R0 register. We get  **0xc1000000**. Bits 9 is zero so processor not perform stack alignment. Press Step (s).
6. We return from **SVC** exception using **BX** instruction. Press Step (s). 
7. We again read **MSP** to R0 register. **MSP** value is **0x20001800**. So processor restore stack pointer to value before exception return. Press Step (s).
8. We push on word to stack. Press Step (s).
9. We read **MSP** to R0 register. **MSP** value is **0x200017fc**. This value is 4 bytes (1 word) lower than previous value. Only to lower bits are zeros. So stack pointer is only 4 bytes aligned.
10. We call **SVC** to trap to handler. Press Step (s).
11. We read **MSP** to R0 register. Now **MSP** value is **0x200017d8**. This 9 words lower that previous value **0x200017fc**. Processor store only 8 words of context. Only extra word is for 8 byte stack alignment. Press Step (s).
12. We load **xPSR** from stack to R0 register. We get  **0xc1000200**. Bits 9 is one so processor perform stack alignment. Processor use this bit to restore stack value on exception return. Press Step (s).
13. We return from **SVC** exception using **BX** instruction. Press Step (s). 
14. We again read **MSP** to R0 register. **MSP** value is **0x200017fc**. So processor correctly restore stack pointer to value before exception return. Press Step (s).

