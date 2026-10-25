# Used MCU
MCU := STM32F042x6

OPT := Release 

# Sources
SRC += $(EXAMPLE_DIR)/main.c
SRC += $(EXAMPLE_DIR)/it.c
SRC += $(EXAMPLE_DIR)/msp.c
SRC += $(EXAMPLE_DIR)/hal_tick.c
SRC += $(THIRD_PARTY_DIR)/cmsis-device-f0/Source/Templates/system_stm32f0xx.c
SRC += $(THIRD_PARTY_DIR)/cmsis-device-f0/Source/Templates/gcc/startup_stm32f042x6.s

SRC += $(THIRD_PARTY_DIR)/stm32f0xx-hal-driver/Src/stm32f0xx_hal.c
SRC += $(THIRD_PARTY_DIR)/stm32f0xx-hal-driver/Src/stm32f0xx_hal_cortex.c
SRC += $(THIRD_PARTY_DIR)/stm32f0xx-hal-driver/Src/stm32f0xx_hal_rcc.c
SRC += $(THIRD_PARTY_DIR)/stm32f0xx-hal-driver/Src/stm32f0xx_hal_pwr.c
SRC += $(THIRD_PARTY_DIR)/stm32f0xx-hal-driver/Src/stm32f0xx_hal_dma.c
SRC += $(THIRD_PARTY_DIR)/stm32f0xx-hal-driver/Src/stm32f0xx_hal_gpio.c
SRC += $(THIRD_PARTY_DIR)/stm32f0xx-hal-driver/Src/stm32f0xx_hal_uart.c
SRC += $(THIRD_PARTY_DIR)/stm32f0xx-hal-driver/Src/stm32f0xx_hal_tim.c
SRC += $(THIRD_PARTY_DIR)/stm32f0xx-hal-driver/Src/stm32f0xx_hal_tim_ex.c


SRC += $(THIRD_PARTY_DIR)/FreeRTOS-Kernel/list.c
SRC += $(THIRD_PARTY_DIR)/FreeRTOS-Kernel/tasks.c
SRC += $(THIRD_PARTY_DIR)/FreeRTOS-Kernel/portable/GCC/ARM_CM0/portasm.c
SRC += $(THIRD_PARTY_DIR)/FreeRTOS-Kernel/portable/GCC/ARM_CM0/port.c
SRC += $(THIRD_PARTY_DIR)/FreeRTOS-Kernel/portable/MemMang/heap_4.c


INC += -I $(EXAMPLE_DIR)
INC += -I $(THIRD_PARTY_DIR)/CMSIS_6/CMSIS/Core/Include
INC += -I $(THIRD_PARTY_DIR)/cmsis-device-f0/Include
INC += -I $(THIRD_PARTY_DIR)/stm32f0xx-hal-driver/Inc
INC += -I $(THIRD_PARTY_DIR)/FreeRTOS-Kernel/include
INC += -I $(THIRD_PARTY_DIR)/FreeRTOS-Kernel/portable/GCC/ARM_CM0

DEFINES += -DUSE_HAL_DRIVER
