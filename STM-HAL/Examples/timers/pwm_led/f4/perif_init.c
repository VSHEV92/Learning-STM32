#include "pwm_led.h"

TIM_HandleTypeDef htim2;

void Peripheral_Init() {

    // Enable Clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_TIM2_CLK_ENABLE();

    // Initialize GPIO pin
    GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin       = GPIO_PIN_5,             // pin number
        .Mode      = GPIO_MODE_AF_PP,        // alternative function push-pull mode
        .Pull      = GPIO_NOPULL,            // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
        .Alternate = GPIO_AF1_TIM2,          // connect GPIO pin to Timer 2

    };
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // Initialize Timer
    htim2.Instance               = TIM2;                            // set timer instance  
    htim2.Init.Prescaler         = 1600 - 1;                        // set timer clock prescaler
    htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;              // set counter to UP mode 
    htim2.Init.Period            = 100 - 1;                         // set timer period
    htim2.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;          // set timer clock division of sampling clock 
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;  // set disable shadow preload register 
    HAL_TIM_Base_Init(&htim2);

    // Configure output capture mode
    TIM_OC_InitTypeDef OC_Config = {
        .OCMode     = TIM_OCMODE_PWM1,     // set output to PWM mode
        .Pulse      = 0,                   // timer capture value
        .OCPolarity = TIM_OCPOLARITY_HIGH, // don't invert OC output 
    };
    HAL_TIM_OC_ConfigChannel(&htim2, &OC_Config, TIM_CHANNEL_1);

}

