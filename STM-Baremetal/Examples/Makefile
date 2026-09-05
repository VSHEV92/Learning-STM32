# Example settings
ifndef example
$(error 'example' variable is not set)
endif

EXAMPLE_DIR := $(abspath $(example))

include $(example)/config.mk

EXECUTABLE_NAME := $(example).elf
OBJDUMP_NAME := $(example).dump


# Toolchain
CROSS_COMPILE := arm-none-eabi-


# GCC flags
CFLAGS += -mthumb
CFLAGS += -mcpu=cortex-m0
CFLAGS += -mfloat-abi=soft
 
CFLAGS += -O0
CFLAGS += -g3
 
CFLAGS += -ffreestanding
CFLAGS += -fno-builtin


# Linker flags
LDFLAGS += -nostartfiles
LDFLAGS += -nostdlib
LDFLAGS += -nodefaultlibs

LDFLAGS += -T Common/link.ld


# Targets
all: clean objdump run

$(EXECUTABLE_NAME): $(SRC)
	$(CROSS_COMPILE)gcc $(CFLAGS) $(LDFLAGS) -o $@ $(SRC)

objdump: $(EXECUTABLE_NAME)
	$(CROSS_COMPILE)objdump -d $(EXECUTABLE_NAME) > $(OBJDUMP_NAME)

run: $(EXECUTABLE_NAME)
	gdb-multiarch $(EXECUTABLE_NAME) -x Common/gdbinit $(GDB_COMMANS)

clean:
	rm -rf $(EXECUTABLE_NAME)

.PHONY: clean objdump run

