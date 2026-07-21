# STM32 HAL Examples

------

### Makefile targets

- **third_party** - clone all needed third party repositories from GitHub 
- **example.elf** - build example ELF file
- **flash** - load binary to STM32
- **all** - build and flash
- **clean** - delete all temporary files

------

### Makefile Run

You need variable define named **example**.  

Each example can be run on different STM families. Each family has it's own configuration file. 

This variable is a path to directory with example **config.mk** file.

For example to build and program SPI MAX7219 example for F4 family run:

```
make all example=spi/max7219_blink/f4
```

