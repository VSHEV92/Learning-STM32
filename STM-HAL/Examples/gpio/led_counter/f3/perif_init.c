#include "led_counter.h"

void Peripheral_Init() {
    // Enable GPIO clocks
    __HAL_RCC_GPIOE_CLK_ENABLE();

    // Initialize GPIO pins
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin   = GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_12;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    
}
