#include "button_exti.h"

void Peripheral_Init() {

    // Enable GPIO Clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Initialize GPIO Led Pin
    GPIO_InitTypeDef GPIO_InitStruct_Led = {
        .Pin   = GPIO_PIN_5,             // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct_Led);
    
    // Initialize GPIO Button Pin
    GPIO_InitTypeDef GPIO_InitStruct_Button = {
        .Pin   = GPIO_PIN_0,             // pin number
        .Mode  = GPIO_MODE_IT_RISING,    // external interrupt on rising edge mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
    };
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct_Button);

    // Configure interrupts to maximum priority (lowest value)
    HAL_NVIC_SetPriority(
        EXTI0_IRQn, // IRQ number
        0,          // Preempt Priority
        0           // Sub Priority
    );
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}
