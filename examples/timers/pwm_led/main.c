#include "pwm_led.h"

TIM_HandleTypeDef htim2;

void main() {

    /*
     * 1. Enable instruction and data cache. Enable instruction prefetch.
     * 2. Set NVIC priority grouping to 4 bits 
     * 3. Call HAL_InitTick(), which:
     *    - set Systick to 1ms period
     *    - set Systick IRQ priority to TICK_INT_PRIORITY (minimum).
     */
    HAL_Init();

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

    // Start timer in PWM mode 
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

    // Update PWM compare value
    uint32_t pulse_width = 0;
    while (1) {
	    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse_width++);
        pulse_width %= 100;
	    HAL_Delay(10);
    }
}

