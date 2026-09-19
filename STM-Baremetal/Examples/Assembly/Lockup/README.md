# Lockup Example

------

### Abstract

This example show how Lockup behavior.  

------

### How to Run

```
 make example=Assembly/Lockup
```

------

### Example Description

This example implements two ISR Vector Handlers: Reset and HardFault. Reset Handler just jump to **main** label. HardFault call **UDF** instruction to cause **Lockup**.



After run GDB opens and halt processor on first instruction. All special registers will be moved to R0 register for illustration. 

**Configure Exceptions and Stack Configuration.**

1. Before Reset R0 has some UNKNOWEN value. In STM32F0 core after reset R0 value is ones. Press Step (s).
2. We call **UDF** (undefined) instruction. This cause processor to trap to **HardFault** handler. Press Step (s).
3. We read **IPSR** to register R0. We get value  **0x3**. So we can check that we in **HardFault** handler. Press Step (s).
4. We call **UDF** (undefined) instruction again to cause Lockup. Press Step (s).
5. Now we in **Lockup State**. Process try to execute instruction from **0xFFFFFFFE** address. This address is marked as **XN**, resulting in a further Lockup instruction that keeps the processor in **Lockup state**.
