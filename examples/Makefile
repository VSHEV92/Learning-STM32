
# Target MCU settings
MCU_FAMILY := f4
MCU := F411

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

# Sources
SRC_FILES := $(addprefix $(EXAMPLE_DIR),$(EXAMPLE_SOURCES))
INC_DISR := $(EXAMPLE_DIR)

all: clean $(THIRD_PARTY_REPOS)

# ---------- Third Party Targets -----------
third_party:
	mkdir third_party

$(STM32CUBE_REPO): third_party
	cd third_party; git clone https://github.com/STMicroelectronics/stm32$(MCU_FAMILY)xx-hal-driver.git

$(CMSIS_REPO): third_party
	cd third_party; git clone https://github.com/ARM-software/CMSIS_6.git

clean:
	rm -rf third_party

.PHONY: clean
