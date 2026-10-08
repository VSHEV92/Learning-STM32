# Example settings
ifndef example
$(error 'example' variable is not set)
endif

EXAMPLE_DIR := $(abspath $(example))
THIRD_PARTY_DIR := $(abspath third_party)

include $(example)/config.mk

EXECUTABLE_NAME := $(example).elf
OBJDUMP_NAME := $(example).dump
MAP_NAME := $(example).map
MAP_PARSE_NAME := $(example).html


# Toolchain
CROSS_COMPILE := arm-none-eabi-


# GCC flags
CFLAGS += -mthumb
CFLAGS += -mcpu=cortex-m0
CFLAGS += -mfloat-abi=soft
 
CFLAGS += -O0
CFLAGS += -g
 
CFLAGS += -ffreestanding
CFLAGS += -fno-builtin

CFLAGS += -ffunction-sections

CFLAGS += -Wall
CFLAGS += -Werror

# Linker flags
ifdef MCU
LDFLAGS += --specs=nano.specs 
LDFLAGS += --specs=nosys.specs
LDFLAGS += -T Common/link_$(MCU).ld
DEFINES += -D$(MCU)
else
LDFLAGS += -nostartfiles
LDFLAGS += -nostdlib
LDFLAGS += -nodefaultlibs
LDFLAGS += -T Common/link.ld
endif

LDFLAGS += -Wl,-Map=$(MAP_NAME)
LDFLAGS += -Wl,--gc-sections

# Objdump sections
OBJDUMP_SECTIONS += -j .isr_vector
OBJDUMP_SECTIONS += -j .text
OBJDUMP_SECTIONS += -j .data

# Targets
all: clean third_party objdump run

$(EXECUTABLE_NAME): $(SRC)
	$(CROSS_COMPILE)gcc $(CFLAGS) $(LDFLAGS) $(DEFINES) $(INC) -o $@ $(SRC)

objdump: $(EXECUTABLE_NAME)
	$(CROSS_COMPILE)objdump -D $(OBJDUMP_SECTIONS) $(EXECUTABLE_NAME) > $(OBJDUMP_NAME)

run: $(EXECUTABLE_NAME)
	gdb-multiarch $(EXECUTABLE_NAME) -x Common/gdbinit_load $(GDB_COMMANS)

connect: $(EXECUTABLE_NAME)
	gdb-multiarch $(EXECUTABLE_NAME) -x Common/gdbinit_connect

flash: clean third_party objdump
	openocd -f interface/stlink.cfg -f target/stm32f0x.cfg -c "program $(EXECUTABLE_NAME) verify reset exit"

size: $(EXECUTABLE_NAME)
	gcc-mapfile-tool $(MAP_NAME) --output=$(MAP_PARSE_NAME) --format=html --sortby=length --order=desc
	$(CROSS_COMPILE)size -A -d $(EXECUTABLE_NAME)
	

monitor:
	pyserial-miniterm /dev/ttyACM0  9600


clean:
	rm -rf $(EXECUTABLE_NAME)
	rm -rf $(OBJDUMP_NAME)
	rm -rf $(MAP_NAME)
	rm -rf $(MAP_PARSE_NAME)

third_party:
	mkdir third_party
	cd third_party; git clone https://github.com/ARM-software/CMSIS_6.git
	cd third_party; git clone https://github.com/STMicroelectronics/cmsis-device-f0.git

.PHONY: clean objdump run flash

