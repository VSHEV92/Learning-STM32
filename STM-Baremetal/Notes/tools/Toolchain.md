# ARM Toolchain

------

- ### Installation

```
sudo apt install gcc-arm-none-eabi -y
sudo apt install gdb-multiarch -y
```

------

### Architecture Options

- **-march=armv6-m** - instruction set for Cortex-M0+
- **-march=armv7-m** - instruction set for  Cortex-M3
- **-march=armv7e-m** - instruction set for Cortex-M4 and M7 (e - Enhanced DSP)
- **-mthumb** - don't use ARM instruction set. Thumb or Thumb2 only
- **-mcpu=cortex-m4** - target Core for compiler (same options for other cores)
- **-mfloat-abi=soft** - software emulation of float point operations
- **-mfloat-abi=hard** - full hardware support for float point operations
- **-mfpu=fpv4-sp-d16** - Float Point Uint architecture. Version 4, single precision, 16 registers of 64 bits (Cortex-M3, Cortex-M4, Cortex-L4, Cortex-G4)
- **-mfpu=fpv5-d16** - Float Point Uint architecture. Version 5, single and double precision, 16 registers of 64 bits (Cortex-F7, Cortex-H7)

------

### Compiler Options

- **-fstack-usage** - generate stack usage file for each function
- **-ffunction-sections** - map each functions for it's own sections
- **-fdata-sections** - map each global data for it's own sections

------

### Linked Options

- **-nostartfiles** - don't use default startup files (crt0.o, crt1.o, crti.o, crtend.o)
- **-nostdlib** - don't link standart library (libc)
- **-Map** - generate Map-file. This file contain all symbols addresses and sizes
- **--gc-sections** - garbage collection. Remove unused symbols from sections

------

### Library C Options

- **--specs=nano.specs** - use newlib-nano as C library
- **--specs=nosys.specs** - there us no system call from OS. Use weak stubs as system calls

------

### Message Options

- **-Wall** - generate message for all Warnings
- **-Werror** - convert every Warning to Error

