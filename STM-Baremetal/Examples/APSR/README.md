# APSR Flags Example

------

### Abstract

This example show what is **APSR** register and explain all it's flags. We show example how to set all of APSR flags:

- **N** - Negative Flag
- **Z** - Zero Flag
- **C** - Carry Flag
- **V** - Overflow Flag

------

### How to Run

```
 make example=Reset_Sequence
```

------

### Example Description

In this example Reset Handler just jump to **main** label.

After run GDB opens and halt processor on first instruction. All special registers will be moved to R0 register for illustration.  R0 register will be displayed after each Step.

1.  Before Reset R0 has some UNKNOWEN value. In STM32F0 core after reset R0 value is ones. Press Step (s).

**Set Negative Flag**

1.  We want to set Negative Flag. This bit is when result of operation is negative. This is when MSB of operation result is one. We move zero value to R0 and add -1 to this register. The result is -1. Press Step (s) two times.
2.  We read **APSR** resiter to R0. It's value is **0x80000000**. Bit 31 is set. This Is **N** bit. 

**Set Zero Flag**

1.  We want to set Zero Flag. This bit is when result of operation is zero. This is when bits of operation result are zeros. We move zero value to R0. Press Step (s).
2.  We read **APSR** resiter to R0. It's value is **0x40000000**. Bit 30 is set. This Is **Z** bit. 

**Set Carry Flag**

1.  We want to set Carry Flag. This bit is when result of operation has carry or borrow bits. Or when bit with value of 1 is shifted out from register. We move value of 0x3 to R0.  So to least bits are ones. Then we shift this value one position right. LSB is shifted out. Press Step (s) two times.
2.  We read **APSR** resiter to R0. It's value is **0x20000000**. Bit 29 is set. This Is **C** bit. 

**Set OVerflow Flag**

1.  We want to set Overflow Flag. This bit is when under signed arithmetic operations. When operand's signs are equal (both pluses or minuses), but result has opposite sign, then overflow occurred. First we store to R0 most positive value - MSB (sign) bit is zero, other bits are ones. Press Step (s) three times.
2.  Then we add 1 to this value. We get result value equal to  0x80000000. The MSB of this value is one. So sign of result (minus) not equal to signs of operands (pluses). Press Step (s).
3.  We read **APSR** resiter to R0. It's value is **0x90000000**. Bit 31 and 28 are set. This are **N** and **V** bits. Bit **N** set because MSB is one. BIt **V** is set because of overflow. 



