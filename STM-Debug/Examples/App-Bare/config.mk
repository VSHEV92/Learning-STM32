# Used MCU
MCU := STM32F091xC

#OPT := Release 

# Sources
SRC += main.c
SRC += it.c
SRC += msp.c
SRC += $(THIRD_PARTY_DIR)/cmsis-device-f0/Source/Templates/system_stm32f0xx.c
SRC += $(THIRD_PARTY_DIR)/cmsis-device-f0/Source/Templates/gcc/startup_stm32f091xc.s

SRC += $(THIRD_PARTY_DIR)/stm32f0xx-hal-driver/Src/stm32f0xx_hal.c
SRC += $(THIRD_PARTY_DIR)/stm32f0xx-hal-driver/Src/stm32f0xx_hal_cortex.c
SRC += $(THIRD_PARTY_DIR)/stm32f0xx-hal-driver/Src/stm32f0xx_hal_rcc.c
SRC += $(THIRD_PARTY_DIR)/stm32f0xx-hal-driver/Src/stm32f0xx_hal_pwr.c
SRC += $(THIRD_PARTY_DIR)/stm32f0xx-hal-driver/Src/stm32f0xx_hal_dma.c
SRC += $(THIRD_PARTY_DIR)/stm32f0xx-hal-driver/Src/stm32f0xx_hal_gpio.c
SRC += $(THIRD_PARTY_DIR)/stm32f0xx-hal-driver/Src/stm32f0xx_hal_uart.c
SRC += $(THIRD_PARTY_DIR)/stm32f0xx-hal-driver/Src/stm32f0xx_hal_tim.c
SRC += $(THIRD_PARTY_DIR)/stm32f0xx-hal-driver/Src/stm32f0xx_hal_tim_ex.c


INC += -I .
INC += -I $(THIRD_PARTY_DIR)/CMSIS_6/CMSIS/Core/Include
INC += -I $(THIRD_PARTY_DIR)/cmsis-device-f0/Include
INC += -I $(THIRD_PARTY_DIR)/stm32f0xx-hal-driver/Inc

DEFINES += -DUSE_HAL_DRIVER
