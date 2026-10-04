# Exception Model Example

------

### Abstract

This example show how exception entry and return work in ARM processors. We look at stack under exception trap from Thread and Handler modes.  

------

### How to Run

```
 make example=CMSIS-Core/Exception_Model
```

------

### Example Description

This example implements to ISR Vector Handlers: Reset, SVC and PendSV. Reset Handler just jump to **main** label. Other handler cause other nested exception and check context in stack.

Thread mode use **PSP**, set exception priorities, initialize register with some values and set **PendSV**.

First we trap to **PendSV** exception by setting it's pending bit. Inside it's handler we call **SVC** instruction to trap to **SVC** handler. 

Handler's functions defined with **naked** attribute. This is because we want to explore stack state after exception entry.

After run GDB opens and halt processor on first instruction.

1.  Break point is set to first **main()** instruction. So you can bypass startup by pressing Continue (c).
2.  We setup interrupt priorities. We set **PendSV** priority to lower value, and **SVC** priority highest.  For that we use **CMSIS** function **NVIC_SetPriority()**. Press Next (**n**) two times.
3.  Next we configure **PSP** value. We set is to address 1 KB below top of RAM. For that we use **_end_of_stack** symbol and **__set_PSP()** function. Press Next (**n**) three times. 
4.  Next we switch to **PSP**. We set **SPSEL** bit in **CONTROL** register using **__set_CONTROL()** function. After that we need to flush processor pipeline by ISB barrier. Press Next (**n**) two times. 
5.  We can check current stack pointer value using command  **p/x $sp**. As expected we get value of **PSP** equal to **0x20001400**.
6.  Next we want to trigger **PendSV** exception. For that we need to assert it's pending bit. We use System Control Block (**SCB**) structure pointer and it's field **ICSR**. Press Continue (c).
7.  Now we inside **PendSV** handler. We want to check current value of **PSP**. Press Next (**n**) two times. **PSP** value is now store in **val** variable and equal to **0x200013e0**. This is 32 byte lower then previous value. Processor store 8 words on exception entry.
8.  We can loop at words saved by exception entry using command  **x/8xw 0x200013e0**. We get:
    - R0 - 0xe000ed00
    - R1 - 0x00000000
    - R2 - 0x10000000
    - R3 - 0xe000ed00
    - R12 - 0xffffffff, we don't use R12 register so it has it's reset value of all ones
    - LR -0x08000163
    - Return Address - 0x080001a2, this is NOP instruction, part of ended busy loop
    - xPSR - 0x01000000, one Thumb bit (T) is set
9.  We call next handler using **SVC** instruction.  Press Next (**n**) 
10.  Now we inside **SVC** handler and again we want to inspect process stack. Before trap we was in Handler mode, so we use **MSP** as stack pointer. Press Next (**n**). **MSP** value is now store in **val** variable and equal to **0x200017d0**.  This value is 48 bytes lower then top of RAM. 32 bytes is because 8 word are stored on exception entry to **SVC** handler. Other 16 bytes is stored on entry in **main** function.
11.  We can loop at words saved by exception entry using command  **x/8xw 0x200013e0**. We get:
     - R0 - 0xe000ed00
     - R1 - 0x00000000
     - R2 - 0x200013e0
     - R3 - 0x20000000
     - R12 - 0xffffffff, we don't use R12 register so it has it's reset value of all ones
     - LR -0xfffffffd - this is exception **return value** for **PendSV** handler
     - Return Address - 0x08000128, this is instruction right after **SVC** instruction in **PendSV** handler
     - xPSR - 0x0100000e,  Thumb bit (T) is set, IPSR is equal to 0xe - PendSV exception number
12.  Next we check exception return value of **SVC** handler using command **p/x $lr**. We get **0xfffffff1**. Upper 4 bits is all ones, so this is a exception return value. Lower 4 bits is equal to **0x1**. This mean we will back to **Handler Mode**. Exception return gets state from the **Main stack**. On return execution uses the **Main Stack**.
13.  Press Next (**n**) to return from **SVC** handler.
14.  Next we check exception return value of **PendSV** handler using command **p/x $lr**. We get **0xfffffffd**. Upper 4 bits is all ones, so this is a exception return value. Lower 4 bits is equal to **0xd**. This mean we will back to **Thread Mode**. Exception return gets state from the **Process stack**. On return execution uses the **Process Stack**.
15.  Press Next (**n**) to return from **PendSV** handler.

 
