MCU_FAMILY := f0
MCU := F042
MCU_HAL := F042x6
CORE := m0
FLOAT_ABI := soft

# Sources
EXAMPLE_SOURCES += main.c
EXAMPLE_SOURCES += f0/perif_init.c

# GCC Options
CFLAGS += -Os -ffunction-sections -fdata-sections -Wl,--gc-sections 

HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_cortex.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_rcc.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_rcc_ex.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_flash.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_flash_ex.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_gpio.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_dma.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_uart.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_i2c.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_i2c_ex.c
