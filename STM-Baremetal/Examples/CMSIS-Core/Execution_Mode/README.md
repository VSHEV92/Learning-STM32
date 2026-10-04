# Execution Mode Example

------

### Abstract

This example show what is Thread and Handler Modes. **Thread Mode** is normal execution mode of processor. It runs in this mode after reset. Processor switches to **Handler Mode** when it trap to exception handler. 

------

### How to Run

```
make example=CMSIS-Core/Execution_Mode
```

------

### Example Description

This example implements to ISR Vector Handlers: Reset, HardFault, SVC and PendSV. Reset Handler just jump to **main** label. Other handler cause other nested exception and check processor mode by reading **IPSR** register.

First we trap to **PendSV** exception by setting it's pending bit. Inside it's handler we call **SVC** instruction to trap to **SVC** handler. Inside next handler we call **UDF** (undefined) instruction to cause **HardFault**. 

Handler function for **HardFault** defined with naked attribute. This is because we need to change Return address of handler to next instruction after **UDF**. Return address is inside **MSP** and we don't want to function entry change stack pointer.

After run GDB opens and halt processor on first instruction.

1.  Break point is set to first **main()** instruction. So you can bypass startup by pressing Continue (c).
2.  We read **IPSR** register to **val** variable. **IPSR** contain Exception number of currently processed handler. Press Next (**n)**. The value of **val** is zero mean that no handler is processed and processor in **Thread mode**. 
3.  We setup interrupt priorities. We set **PendSV** priority to lower value, and **SVC** priority highest.  For that we use **CMSIS** function **NVIC_SetPriority()**. Press Next (**n**) two times.
4.  Next we want to trigger **PendSV** exception. For that we need to assert it's pending bit. We use System Control Block (**SCB**) structure pointer and it's field **ICSR**. Press Continue (c).
5.  Now we inside **PendSV** handler. We read **IPSR** register to **val** variable. This variable is **0xe** or **14** in decimal. This is equal to number of **PendSV** exception. Press Next (**n**) two times.
6.  Next we call **SVC** using inline assembly. Press Next (**n**) .
7.  We inside **SVC** handler. We read **IPSR** register to **val** variable. This variable is **0xb** or **11** in decimal. This is equal to number of **SVC** exception. So we really preempt **PendSV** handler and trap to **SVC** handler. Press Next (**n**) two times.
8.  Next we call undefined instruction (**UDF**) using inline assembly to trap to **HardFault**. Press Next (**n**) .
9.  We inside **HardFault** handler. We read **IPSR** register to **val** variable. This variableis **0x3**. This is equal to number of **HarFault** exception. We can not just return from this handler because it's return address points to **UDF** instruction. To continue program execution we need to change return address. Press Next (**n**) .
10.  First we get top of stack by reading **MSP**. Then we get return address by offset of 6 words from top of stack. We increment return address by 2 to point to next instruction after **UDF**. We store updated return address back to stack. Press Next (**n**)  five times.
11.   Handler of **HardFault** is defined with attribute **naked**. So we need to use explicit call of branch instruction to return from handler. Press Next (**n**).  
12.  Now again in **SVC** handler. We read **IPSR** register to **val** variable. This variable is **0xb** or **11** in decimal. So we really return to **SVC** handler. Press Next (**n**) two times.  
13.  Now again in **PendSV** handler. We read **IPSR** register to **val** variable. This variable is **0xe** or **14** in decimal. So we really return to **PendSV** handler. Press Next (**n**) two times.  
14.  Now we return from all handler. We read **IPSR** register to **val** variable. Press Next (**n)**. The value of **val** is zero mean that no handler is processed and processor in **Thread mode**. 

