#include <stm32f0xx.h>

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim14;

extern SemaphoreHandle_t semaphore;

void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim2);
}

void TIM14_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim14);

    BaseType_t woken_flag = pdFALSE;
    xSemaphoreGiveFromISR(semaphore, &woken_flag);
    portYIELD_FROM_ISR(woken_flag);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        HAL_IncTick();
    }
}


