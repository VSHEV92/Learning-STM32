# Used MCU
MCU := STM32F042x6

# Sources
SRC += $(EXAMPLE_DIR)/main.c
SRC += $(THIRD_PARTY_DIR)/cmsis-device-f0/Source/Templates/system_stm32f0xx.c
SRC += $(THIRD_PARTY_DIR)/cmsis-device-f0/Source/Templates/gcc/startup_stm32f042x6.s
#SRC += $(EXAMPLE_DIR)/startup.S

INC += -I $(THIRD_PARTY_DIR)/CMSIS_6/CMSIS/Core/Include
INC += -I $(THIRD_PARTY_DIR)/cmsis-device-f0/Include

