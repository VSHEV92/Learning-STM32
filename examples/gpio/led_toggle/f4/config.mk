# Target configurations

# Sources
EXAMPLE_SOURCES += main.c

HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_cortex.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_rcc.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_flash.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_flash_ex.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_pwr.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_pwr_ex.c
HAL_SOURCES += $(STM32HAL_REPO)/Src/stm32$(MCU_FAMILY)xx_hal_gpio.c
