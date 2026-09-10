# Stack Pointer Example

------

### Abstract

This example show how stack pointer can be used. We start by using MSP, Then we switch to PSP, At the end we trap to SVC handler and loop at stack after exception entry.

------

### How to Run

```
 make example=Stack_Pointer
```

------

### Example Description

This example implements to ISR Vector Handlers: Reset and SVC. Reset Handler just jump to **main** label. SVC handler inspect values from in stack of trapped process and then jump back.

After run GDB opens and halt processor on first instruction. All special registers will be moved to R0 register for illustration.  R0 register will be displayed after each Step.

**Inspect MSP.**

1. Before Reset R0 has some UNKNOWEN value. Press Step (s).
2. In STM32F0 core after reset R0 value is ones. Press Step (s).
3. We read **CONTROL** register to R0. It's value also zero, CONTROL has to flags. Second bit is **SPSEL**, that show which SP bank is used. This bit is zero so we use MSP.  Press Step (s).
4. We read value of **MSP** to R0. It's value is 0x20001800. This is top address of RAM in STM32F042k6. This value is equal to value of first ISR Vector table entry. Press Step (s).
5. We read value of **SP** register to R0. It's value is also 0x20001800. Press Step (s).
6. Next we fill registers R0 to R3 by values from 0x0 to 0x3. Then we pop this registers to stack. Press Step (s) five times.
7. Next we again read value of **SP** register to R0. It's value is also 0x200017f0. This is 16 bytes lower than it was. Press Step (s). 
8. We load values from stack to R0  using top of stack and offset. Check that this values are same that we moved to registers before.  Press Step (s) four times.

**Inspect PSP.**

1. We start switching to Process Stack pointer. We will set PSP to 1 KB lower then top address of RAM in STM32F042k6. First of all we store value 1024 to R1. Then we store top address of RAM to R0. We subtract R1 ffrom R0 and get address of **PSP**. At the end to move this address to **PSP**. Press Step (s) four times.
2. We switch to **PSP**. For that we need to set **SPSEL** bit in **CONTROL** register. We set bit[1] in register R0. We move R0 to **CONTROL** register. This operation switch stack pointer to **PSP**. Press Step (s) two times.
3. We update **CONTROL** register. Some instruction can by fecthed to process pipeline and use old stack pointer value. Some we need to run instruction synchronization barrier (**ISB**) to flush processor pipeline. Press Step (s).
4. Read **CONTROL** register to R0 and check that **SPSEL** bit is set. Now we use **PSP**. Press Step (s).
5. We read value of **PSP** register to R0. It's value is also 0x20001400. This is 1KB lower then top address of RAM. Press Step (s).
6. We read value of **SP** register to R0. It's value is also 0x20001400. Press Step (s).
7. Next we fill registers R0 to R3 by values from 0xF0 to 0xF3. Then we pop this registers to stack. Press Step (s) five times.
8. Next we again read value of **SP** register to R0. It's value is also 0x200013f0. This is 16 bytes lower than it was. Press Step (s). 
9. We load values from stack to R0  using top of stack and offset. Check that this values are same that we moved to registers before.  Press Step (s) four times.
10. we restore R0 value to 0xF0. Press Step (s).

**Inspect Stack inside Exception Handler.**

1. We call SVC instruction to trap to SVC handler. Press Step (s).
2. Now we inside handler. In Handle mode **MSP** is always used as stack pointer. We read **CONTROL** register to R0. It's value also zero, CONTROL has to flags. Second bit is **SPSEL**, that show which SP bank is used. This bit is zero so we use **MSP**.  Press Step (s).
3. Next we  read value of **MSP** register to R0. It's value is also 0x200017f0. This is the old value of stack pointer before we switch to **PSP**. Press Step (s). 
4. We read value of **SP** register to R0. It's value is also 0x200017f0. Press Step (s).
5. At exception entry some data automatically pushed to trap process stack. We use PSP before, so this this data is pushed there. We store value from **PSP** register to R1. Press Step (s).
6. Now we can inspect trapped process stack using PSP address in R1 and offset. First four registers are R0 to R3. This registers have values from 0xF0 to 0xF3, We can check this. Step (s) four times.
7. Next to stack values is registers R12 and R14 (**LR**). We don't use this registers before, so they have reset values and are all ones. Press Step (s) two times.
8. Next stack value is **Return Address**. This value is equal 0x80000a8. This is address of instruction right after SVC instruction. Press Step (s).
9. Last value is **xPSR** register value of trapped process. It's value is 0x21000000. So bit **xPSR.C** is set. And also bit **xPSR.T** is set, so we in **Thumb state**. Press Step (s).
10. Next we return from handler using BX instruction. Press Step (s)
11. At the and we again read data from **PSP**. We load values from stack to R0  using top of stack and offset. Check that this values are steel from 0xF0 to 0xF3.  Press Step (s) four times.

