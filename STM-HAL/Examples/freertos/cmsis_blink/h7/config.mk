# Target configurations
MCU_FAMILY := h7
MCU := H755
MCU_HAL := H755xx
CORE := m7
FLOAT_ABI := hard
FLOAT_ARCH := fpv4-sp-d16

# FreeRTOS 
USE_FREERTOS := 1
INC_DIRS += -I$(STM32FREERTOS_REPO)/Source/portable/GCC/ARM_CM7/r0p1
SRC_FILES += $(STM32FREERTOS_REPO)/Source/portable/GCC/ARM_CM7/r0p1/port.c
SRC_FILES += $(STM32FREERTOS_REPO)/Source/portable/MemMang/heap_4.c

# Sources
EXAMPLE_SOURCES += main.c
EXAMPLE_SOURCES += h7/perif_init.c
EXAMPLE_SOURCES += h7/hal_timer.c

HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_cortex.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_rcc.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_flash.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_flash_ex.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_gpio.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_dma.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_tim.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_tim_ex.c

DEFINES += -DCORE_CM7

