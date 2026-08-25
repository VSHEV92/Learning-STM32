#include "oc_toggle.h"

TIM_HandleTypeDef htim1;

void Peripheral_Init() {

    // Enable Clocks
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_TIM1_CLK_ENABLE();

    // Initialize GPIO pin
    GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin       = GPIO_PIN_14,            // pin number
        .Mode      = GPIO_MODE_AF_PP,        // alternative function push-pull mode
        .Pull      = GPIO_NOPULL,            // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
        .Alternate = GPIO_AF2_TIM1,          // connect GPIO pin to Timer 2

    };
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    
    // Initialize Timer
    htim1.Instance               = TIM1;                            // set timer instance  
    htim1.Init.Prescaler         = 8000 - 1;                        // set timer clock prescaler
    htim1.Init.CounterMode       = TIM_COUNTERMODE_UP;              // set counter to UP mode 
    htim1.Init.Period            = 500 - 1;                         // set timer period
    htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;          // set timer clock division of sampling clock 
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;  // set disable shadow preload register 
    HAL_TIM_Base_Init(&htim1);

    // Configure output capture mode
    TIM_OC_InitTypeDef OC_Config = {
        .OCMode     = TIM_OCMODE_TOGGLE,   // toggle output on capture value
        .Pulse      = 0,                   // timer capture value
        .OCPolarity = TIM_OCPOLARITY_HIGH, // don't invert OC output 
    };
    HAL_TIM_OC_ConfigChannel(&htim1, &OC_Config, TIM_CHANNEL_4);

}



