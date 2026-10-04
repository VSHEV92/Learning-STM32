# Stack Pointer Example

------

### Abstract

This example show how stack pointer can be used. We start by using MSP, Then we switch to PSP, At the end we trap to SVC handler and loop at stack after exception entry.

------

### How to Run

```
make example=CMSIS-Core/Stack_Pointer
```

------

### Example Description

This example implements to ISR Vector Handlers: Reset and SVC. Reset Handler just jump to **main** label. SVC handler inspect values from in stack of trapped process and then jump back.

After run GDB opens and halt processor on first instruction.

1.  Break point is set to first **main()** instruction. So you can bypass startup by pressing Continue (c).
2.  We read **CONTOL** register to **val** variable. It's value is zero. **CONTROL** has two flags. Second bit is **SPSEL**, that show which SP bank is used. This bit is zero so we use MSP. Press Next (**n)**. 
3.  We read **MSP** register to **val** variable. It's value is **0x200017e0**. This is 32 bytes below address of top of RAM. Stack pointer was modified by 32 bytes on main function entry.
4.  Using command **p/x $sp** check that SP register also has value **0x200017e0**. Press Next (**n)**. 
5.  Now we push registers R0-R3 to stack. Using command **p/x $sp** check that SP register has value **0x200017d0**. This is 16 bytes or 4 words below previous value. Press Next (**n)**. 
6.  We start switching to Process Stack pointer. We will set **PSP** to 1 KB lower then top address of RAM in STM32F042k6. To find top of RAM we use symbol **_end_of_stack**. We update **PSP** value using function **__set_PSP()**. Press Next (**n)** three times.
7.  We switch to **PSP**  by setting bit **SPSEL** in **CONTROL** register. For that we use function **__set_CONTROL()**. Press Next (**n)** two times.  
8.  We read **MSP** register to **val** variable. It's value is **0x20001400**.
9.  Using command **p/x $sp** check that SP register also has value **0x20001400**. So we really switch to **PSP**. Press Next (**n)**.
10.  We again push registers R0-R3 to stack. Using command **p/x $sp** check that SP register has value **0x200013f0**. This is 16 bytes or 4 words below previous value. Press Next (**n)**. 
11.  We call **SVC** instruction to trap to **SVC** handler. Press Next (**n)**.
12.  Now we inside handler. In Handle mode **MSP** is always used as stack pointer. We read **CONTOL** register to **val** variable. Second bit is **SPSEL**, that show which SP bank is used. This bit is zero so we use **MSP**. Press Next (**n)** two times.
13.  We read **MSP** register to **val** variable. It's value is **0x200017b8**. This is 24 bytes lower then old value of stack pointer before we switch to **PSP**. This 24 bytes adder on entry to Ecxeption Handler. Press Next (**n)**
14.  We read **MSP** register to **val** variable. It's value is **0x200013d0**. This is 32 bytes lower then previous value. This is because 8 words was pushed to stack on exception entry. Press Next (**n)**
