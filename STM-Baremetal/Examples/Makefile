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
LDFLAGS += -nostartfiles
LDFLAGS += -nostdlib
LDFLAGS += -nodefaultlibs

LDFLAGS += -T Common/link.ld
LDFLAGS += -Wl,-Map=$(MAP_NAME)
LDFLAGS += -Wl,--gc-sections

# Objdump sections
OBJDUMP_SECTIONS += -j .isr_vector
OBJDUMP_SECTIONS += -j .text
OBJDUMP_SECTIONS += -j .data

# Targets
all: clean third_party objdump run

$(EXECUTABLE_NAME): $(SRC)
	$(CROSS_COMPILE)gcc $(CFLAGS) $(LDFLAGS) $(INC) -o $@ $(SRC)

objdump: $(EXECUTABLE_NAME)
	$(CROSS_COMPILE)objdump -D $(OBJDUMP_SECTIONS) $(EXECUTABLE_NAME) > $(OBJDUMP_NAME)
	gcc-mapfile-tool $(MAP_NAME) --output=$(MAP_PARSE_NAME) --format=html --sortby=length --order=desc

run: $(EXECUTABLE_NAME)
	gdb-multiarch $(EXECUTABLE_NAME) -x Common/gdbinit $(GDB_COMMANS)

clean:
	rm -rf $(EXECUTABLE_NAME)
	rm -rf $(OBJDUMP_NAME)
	rm -rf $(MAP_NAME)
	rm -rf $(MAP_PARSE_NAME)

third_party:
	mkdir third_party
	cd third_party; git clone https://github.com/ARM-software/CMSIS_6.git

.PHONY: clean objdump run

