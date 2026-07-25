# Target configurations
MCU_FAMILY := f4
MCU := F411
MCU_HAL := F411xE
CORE := m4
FLOAT_ABI := hard
FLOAT_ARCH := fpv4-sp-d16

# Sources
EXAMPLE_SOURCES += main.c
EXAMPLE_SOURCES += f4/perif_init.c

HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_cortex.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_rcc.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_flash.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_flash_ex.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_gpio.c
