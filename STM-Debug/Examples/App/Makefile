APP_NAME := app
EXECUTABLE_NAME := $(APP_NAME).elf
OBJDUMP_NAME := $(APP_NAME).dump
MAP_NAME := $(APP_NAME).map
MAP_PARSE_NAME := $(APP_NAME).html

THIRD_PARTY_DIR := $(abspath third_party)

include config.mk

# Toolchain
CROSS_COMPILE := arm-none-eabi-

# GCC flags
CFLAGS += -mthumb
CFLAGS += -mcpu=cortex-m0
CFLAGS += -mfloat-abi=soft
 
CFLAGS += -ffreestanding

CFLAGS += -Wall
CFLAGS += -Werror

CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -fstack-usage

ifdef OPT
CFLAGS += -Os
else
CFLAGS += -O0
CFLAGS += -g
endif

# Linker flags
LDFLAGS += --specs=nano.specs 
LDFLAGS += --specs=nosys.specs

LDFLAGS += -T link_$(MCU).ld

LDFLAGS += -Wl,-Map=$(MAP_NAME)
LDFLAGS += -Wl,--gc-sections

DEFINES += -D$(MCU)

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
	gdb-multiarch $(EXECUTABLE_NAME) -x gdbinit_load $(GDB_COMMANS)

connect: $(EXECUTABLE_NAME)
	gdb-multiarch $(EXECUTABLE_NAME) -x gdbinit_connect

flash: clean third_party objdump
	openocd -f interface/stlink.cfg -f target/stm32f0x.cfg -c "program $(EXECUTABLE_NAME) verify reset exit"

size: $(EXECUTABLE_NAME)
	# MAP file as HTML
	gcc-mapfile-tool $(MAP_NAME) --output=$(MAP_PARSE_NAME) --format=html --sortby=length --order=desc

	# Print Section Size Summary
	$(CROSS_COMPILE)size -A -d $(EXECUTABLE_NAME)

puncover:
	puncover --elf $(EXECUTABLE_NAME) --build-dir .

monitor:
	pyserial-miniterm /dev/ttyACM0  9600


clean:
	rm -rf $(EXECUTABLE_NAME)
	rm -rf $(OBJDUMP_NAME)
	rm -rf $(MAP_NAME)
	rm -rf $(MAP_PARSE_NAME)
	rm -rf *.su
	rm -rf *.expand

third_party:
	mkdir third_party
	cd third_party; git clone https://github.com/ARM-software/CMSIS_6.git
	cd third_party; git clone https://github.com/STMicroelectronics/cmsis-device-f0.git
	cd third_party; git clone https://github.com/STMicroelectronics/stm32f0xx-hal-driver.git
	cd third_party; git clone https://github.com/FreeRTOS/FreeRTOS-Kernel.git
	

.PHONY: clean objdump run flash

