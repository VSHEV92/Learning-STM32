#include "led_toggle.h"

void Peripheral_Init() {

    // Enable GPIO clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Initialize GPIO pin
    GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin   = LED_PIN,                // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

}
