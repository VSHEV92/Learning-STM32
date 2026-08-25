#include "base_it.h"

TIM_HandleTypeDef htim1;

void TIM1_UP_TIM16_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim1);
}

void Peripheral_Init() {

    // Enable Clocks
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_TIM1_CLK_ENABLE();

    // Initialize GPIO pin
    GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin   = GPIO_PIN_15,            // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    
    // Initialize Timer
    htim1.Instance               = TIM1;                            // set timer instance  
    htim1.Init.Prescaler         = 8000 - 1;                        // set timer clock prescaler
    htim1.Init.CounterMode       = TIM_COUNTERMODE_UP;              // set counter to UP mode 
    htim1.Init.Period            = 1000 - 1;                        // set timer period
    htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;          // set timer clock division of sampling clock 
    htim1.Init.RepetitionCounter = 0;                               // set repetiotion value to zero 
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;  // set disable shadow preload register 
    HAL_TIM_Base_Init(&htim1);

    // Setup timer interrupts
    HAL_NVIC_SetPriority( TIM1_UP_TIM16_IRQn, 0, 0 );
    HAL_NVIC_EnableIRQ( TIM1_UP_TIM16_IRQn );
}

