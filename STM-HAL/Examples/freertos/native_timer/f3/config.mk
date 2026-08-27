# Target configurations
MCU_FAMILY := f3
MCU := F303
MCU_HAL := F303xC
CORE := m4
FLOAT_ABI := hard
FLOAT_ARCH := fpv4-sp-d16

# FreeRTOS 
USE_FREERTOS := 1
INC_DIRS += -I$(STM32FREERTOS_REPO)/Source/portable/GCC/ARM_CM4F
SRC_FILES += $(STM32FREERTOS_REPO)/Source/portable/GCC/ARM_CM4F/port.c
SRC_FILES += $(STM32FREERTOS_REPO)/Source/portable/MemMang/heap_4.c

# Sources
EXAMPLE_SOURCES += main.c
EXAMPLE_SOURCES += f3/hal_timer.c
EXAMPLE_SOURCES += f3/perif_init.c

HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_cortex.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_rcc.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_flash.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_flash_ex.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_gpio.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_dma.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_tim.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_tim_ex.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_uart.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_uart_ex.c
