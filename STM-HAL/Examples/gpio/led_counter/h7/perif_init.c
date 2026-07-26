#include "led_counter.h"

void Peripheral_Init() {
    // Enable GPIO clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    // Initialize GPIO pins
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin   = GPIO_PIN_5 | GPIO_PIN_6;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin   = GPIO_PIN_5;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = GPIO_PIN_14;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
