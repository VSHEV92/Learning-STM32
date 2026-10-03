# APSR Example

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
 make example=CMSIS/APSR
```

------

### Example Description

After run GDB opens and halt processor on first instruction.

1.  Break point is set to first **main()** instruction. So you can bypass startup by pressing Continue (c).

**Set Negative Flag**

1.  We want to set Negative Flag. This bit is when result of operation is negative. This is when MSB of operation result is one. We move -1 to variable **dat**. Press Next (n) two times.
2.  We read **APSR** register to variable **val**. It's value is **0x80000000**. Bit 31 is set. This Is **N** bit. 

**Set Zero Flag**

1.  We want to set Zero Flag. This bit is when result of operation is zero. This is when bits of operation result are zeros. We move -1 to variable **dat**. Press Next (n) two times.
2.  We read **APSR**  register to variable **val**. It's value is **0x40000000**. Bit 30 is set. This Is **Z** bit. 

**Set Carry Flag**

1.  We want to set Carry Flag. This bit is when result of operation has carry or borrow bits. Or when bit with value of 1 is shifted out from register. We set variable **udat** to all ones. So to least bit are one. Then we shift this value one position right. LSB is shifted out. Press Next (n) three times.
2.  We read **APSR** register to variable **val**. It's value is **0x20000000**. Bit 29 is set. This Is **C** bit. 

**Set Overflow Flag**

1. We want to set Overflow Flag. This bit is when under signed arithmetic operations. When operand's signs are equal (both pluses or minuses), but result has opposite sign, then overflow occurred. First we store to variable **dat** most positive value - MSB (sign) bit is zero, other bits are ones.  Press Next (n) two times.

2. Then we add 1 to this value. We get result value equal to  0x80000000. The MSB of this value is one. So sign of result (minus) not equal to signs of operands (pluses). Press Next (n) two times.

3. We read **APSR** register to variable **val**. It's value is **0x90000000**. Bit 31 and 28 are set. This are **N** and **V** bits. Bit **N** set because MSB is one. BIt **V** is set because of overflow. 

   
