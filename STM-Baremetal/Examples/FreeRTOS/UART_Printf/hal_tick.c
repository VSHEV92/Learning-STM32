#include <stm32f0xx.h>

TIM_HandleTypeDef        htim2;

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority) {
    RCC_ClkInitTypeDef    clkconfig;
    uint32_t              uwTimclock = 0U;
  
    uint32_t              uwPrescalerValue = 0U;
    uint32_t              pFLatency;
  
    __HAL_RCC_TIM2_CLK_ENABLE();
  
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);
  
    uwTimclock = HAL_RCC_GetPCLK1Freq();
  
    uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);
  
    htim2.Instance = TIM2;
    htim2.Init.Period = (1000000U / 1000U) - 1U;
    htim2.Init.Prescaler = uwPrescalerValue;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  
    HAL_TIM_Base_Init(&htim2);
    HAL_TIM_Base_Start_IT(&htim2);

    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    
    HAL_NVIC_SetPriority(TIM2_IRQn, TickPriority, 0U);
    uwTickPrio = TickPriority;
  
    return HAL_OK;
}


void HAL_SuspendTick(void) {
    __HAL_TIM_DISABLE_IT(&htim2, TIM_IT_UPDATE);
}


void HAL_ResumeTick(void) {
    __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);
}

