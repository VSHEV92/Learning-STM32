# STM32 CUBE IDE Examples

------

### Makefile targets

- **create_project** - create CUBE IDE project first from **.ioc** file, then apply changes in **.patch** flie 
- **build_project** - build release version of project
- **program_project** - load binary to STM32
- **clean** - delete all temporary files

------

### Makefile Run

You need variable define named **example**.  

This variable is a path to directory with example **./ioc** file.

For example to build and program SPI MAX7219 example run:

```
make example=spi/max7219_hart
```

