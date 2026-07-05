# Example settings
ifndef example
$(error 'example' variable is not set)
endif

EXAMPLE_DIR := $(abspath $(example))

include $(EXAMPLE_DIR)/config.mk

# Third Party Repos
CMSIS_REPO := third_party/CMSIS_6
STM32HAL_REPO := third_party/stm32$(MCU_FAMILY)xx-hal-driver
STM32CMSIS_REPO := third_party/cmsis-device-$(MCU_FAMILY)

# Toolchain
CROSS_COMPILE := arm-none-eabi-

# Sources
SRC_FILES += $(addprefix $(EXAMPLE_DIR)/../,$(EXAMPLE_SOURCES))
SRC_FILES += common/$(MCU_FAMILY)/startup.s
SRC_FILES += common/$(MCU_FAMILY)/system.c
SRC_FILES += $(HAL_SOURCES)


#includes
INC_DIRS += -I$(EXAMPLE_DIR)
INC_DIRS += -Icommon/$(MCU_FAMILY)
INC_DIRS += -I$(CMSIS_REPO)/CMSIS/Core/Include
INC_DIRS += -I$(STM32HAL_REPO)/Inc
INC_DIRS += -I$(STM32CMSIS_REPO)/Include

#defines
DEFINES += -DSTM32$(MCU_HAL)

# GCC flags
CFLAGS += -mthumb
CFLAGS += -mcpu=cortex-$(CORE)
CFLAGS += -mfloat-abi=$(FLOAT_ABI)

ifeq ($(FLOAT_ABI), hard)
CFLAGS += -mfpu=$(FLOAT_ARCH)
endif

LDFLAGS += -nostartfiles
LDFLAGS += -nostdlib
LDFLAGS += -T common/$(MCU_FAMILY)/link.ld

all: flash

example.elf: third_party $(SRC_FILES)
	$(CROSS_COMPILE)gcc $(CFLAGS) $(LDFLAGS) $(DEFINES) $(INC_DIRS) -o example.elf $(SRC_FILES)

flash: example.elf
	STM32_Programmer_CLI -c port=SWD mode=UR -w example.elf 
	STM32_Programmer_CLI -c port=SWD -rst

# ---------- Third Party Targets -----------
third_party:
	mkdir third_party
	cd third_party; git clone https://github.com/ARM-software/CMSIS_6.git
	cd third_party; git clone https://github.com/STMicroelectronics/stm32$(MCU_FAMILY)xx-hal-driver.git
	cd third_party; git clone https://github.com/STMicroelectronics/cmsis-device-$(MCU_FAMILY).git

clean:
	rm -rf third_party
	rm -rf example.elf

.PHONY: clean
