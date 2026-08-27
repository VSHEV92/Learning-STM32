#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_tim.h"

TIM_HandleTypeDef        htim1;

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority) {
    RCC_ClkInitTypeDef    clkconfig;
    uint32_t              uwTimclock = 0U;
  
    uint32_t              uwPrescalerValue = 0U;
    uint32_t              pFLatency;
  
    HAL_StatusTypeDef     status;
  
    /* Enable TIM1 clock */
    __HAL_RCC_TIM1_CLK_ENABLE();
  
    /* Get clock configuration */
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);
  
    /* Compute TIM1 clock */
    uwTimclock = HAL_RCC_GetPCLK2Freq();
  
    /* Compute the prescaler value to have TIM1 counter clock equal to 1MHz */
    uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);
  
    /* Initialize TIM1 */
    htim1.Instance = TIM1;
    htim1.Init.Period = (1000000U / 1000U) - 1U;
    htim1.Init.Prescaler = uwPrescalerValue;
    htim1.Init.ClockDivision = 0;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  
    HAL_TIM_Base_Init(&htim1);
    HAL_TIM_Base_Start_IT(&htim1);

    /* Enable the TIM1 global Interrupt */
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
    
    /* Configure the TIM IRQ priority */
    HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, TickPriority, 0U);
    uwTickPrio = TickPriority;
  
    return HAL_OK;
}


void HAL_SuspendTick(void) {
    __HAL_TIM_DISABLE_IT(&htim1, TIM_IT_UPDATE);
}


void HAL_ResumeTick(void) {
    __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);
}

void TIM1_UP_TIM16_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM1) {
        HAL_IncTick();
    }
}

