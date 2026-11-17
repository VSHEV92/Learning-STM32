#include <stm32f0xx.h>
#include <stdio.h>

extern TIM_HandleTypeDef htim14;

void SysTick_Handler(void) {
    HAL_IncTick();
}

void TIM14_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim14);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM14) {
        printf("Hello World\n");
    }
}


