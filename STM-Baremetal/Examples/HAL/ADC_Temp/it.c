#include <stm32f0xx.h>

void SysTick_Handler(void) {
    HAL_IncTick();
}

