# Example settings
ifndef example
$(error 'example' variable is not set)
endif

EXAMPLE_DIR := $(abspath $(example))
DRIVERS_DIR := common/drivers

include $(EXAMPLE_DIR)/config.mk

# Third Party Repos
CMSIS_REPO := third_party/CMSIS_6
STM32HAL_REPO := third_party/stm32$(MCU_FAMILY)xx-hal-driver
STM32CMSIS_REPO := third_party/cmsis-device-$(MCU_FAMILY)
STM32FREERTOS_REPO := third_party/stm32-mw-freertos

# Toolchain
CROSS_COMPILE := arm-none-eabi-

# Sources
SRC_FILES += $(addprefix $(EXAMPLE_DIR)/../,$(EXAMPLE_SOURCES))
SRC_FILES += common/$(MCU_FAMILY)/startup.s
SRC_FILES += common/$(MCU_FAMILY)/system.c
SRC_FILES += common/system.c
SRC_FILES += $(HAL_SOURCES)
SRC_FILES += $(DRIVER_SOURCES)

ifdef USE_FREERTOS
SRC_FILES += common/$(MCU_FAMILY)/cmsis_os2.c
SRC_FILES += $(STM32FREERTOS_REPO)/Source/tasks.c
SRC_FILES += $(STM32FREERTOS_REPO)/Source/timers.c
SRC_FILES += $(STM32FREERTOS_REPO)/Source/list.c
SRC_FILES += $(STM32FREERTOS_REPO)/Source/queue.c
SRC_FILES += $(STM32FREERTOS_REPO)/Source/event_groups.c
SRC_FILES += $(STM32FREERTOS_REPO)/Source/stream_buffer.c

DEFINES += -DUSE_FREERTOS
endif

#includes
INC_DIRS += -I$(EXAMPLE_DIR)
INC_DIRS += -I$(CMSIS_REPO)/CMSIS/Core/Include
INC_DIRS += -I$(STM32HAL_REPO)/Inc
INC_DIRS += -I$(STM32CMSIS_REPO)/Include
INC_DIRS += -I$(STM32FREERTOS_REPO)/Source/CMSIS_RTOS_V2
INC_DIRS += -I$(STM32FREERTOS_REPO)/Source/include

INC_DIRS += -Icommon/$(MCU_FAMILY)
INC_DIRS += -Icommon/drivers/74HC595
INC_DIRS += -Icommon/drivers/MAX7219

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

	cd third_party; git clone https://github.com/STMicroelectronics/stm32f0xx-hal-driver.git
	cd third_party; git clone https://github.com/STMicroelectronics/cmsis-device-f0.git

	cd third_party; git clone https://github.com/STMicroelectronics/stm32f3xx-hal-driver.git
	cd third_party; git clone https://github.com/STMicroelectronics/cmsis-device-f3.git

	cd third_party; git clone https://github.com/STMicroelectronics/stm32f4xx-hal-driver.git
	cd third_party; git clone https://github.com/STMicroelectronics/cmsis-device-f4.git

	cd third_party; git clone https://github.com/STMicroelectronics/stm32h7xx-hal-driver.git
	cd third_party; git clone https://github.com/STMicroelectronics/cmsis-device-h7.git
	
	cd third_party; git clone https://github.com/STMicroelectronics/stm32-mw-freertos.git

clean:
	rm -rf third_party
	rm -rf example.elf

.PHONY: clean all flash example.elf
