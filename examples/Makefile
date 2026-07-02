
# Target MCU settings
MCU_FAMILY := f4
MCU := F411
CORE := m4
FLOAT_ABI := hard
FLOAT_ARCH := fpv4-sp-d16

# Example settings
ifndef example
$(error 'example' variable is not set)
endif

EXAMPLE_DIR := $(abspath $(example))
include $(EXAMPLE_DIR)/config.mk

# Third Party Repos
CMSIS_REPO := third_party/CMSIS_6
STM32CUBE_REPO := third_party/STM32Cube$(MCU)
THIRD_PARTY_REPOS := $(CMSIS_REPO) $(STM32CUBE_REPO)

# Toolchain
CROSS_COMPILE := arm-none-eabi-

# Sources
SRC_FILES += $(addprefix $(EXAMPLE_DIR)/,$(EXAMPLE_SOURCES))
SRC_FILES += common/startup_stm32$(MCU_FAMILY).s
SRC_FILES += common/init_$(MCU_FAMILY).c

#includes
INC_DIRS += -I$(EXAMPLE_DIR)
INC_DIRS += -I$(CMSIS_REPO)/CMSIS/Core/Include
INC_DIRS += -I$(STM32CUBE_REPO)/Inc

#defines
ifeq ($(FLOAT_ABI), hard)
DEFINES += -D__FPU_PRESENT
endif

# GCC flags
CFLAGS += -mthumb
CFLAGS += -mcpu=cortex-$(CORE)
CFLAGS += -mfloat-abi=$(FLOAT_ABI)

ifeq ($(FLOAT_ABI), hard)
CFLAGS += -mfpu=$(FLOAT_ARCH)
endif

LDFLAGS += -nostartfiles
LDFLAGS += -nostdlib
LDFLAGS += -T common/STM32$(MCU).ld

all: example.elf


example.elf: $(THIRD_PARTY_REPOS)
	$(CROSS_COMPILE)gcc $(CFLAGS) $(LDFLAGS) $(DEFINES) $(INC_DIRS) -o example.elf $(SRC_FILES)


# ---------- Third Party Targets -----------
third_party:
	mkdir third_party

$(STM32CUBE_REPO): third_party
	- cd third_party; git clone https://github.com/STMicroelectronics/stm32$(MCU_FAMILY)xx-hal-driver.git

$(CMSIS_REPO): third_party
	- cd third_party; git clone https://github.com/ARM-software/CMSIS_6.git

clean:
	rm -rf third_party
	rm -rf example.elf

.PHONY: clean
