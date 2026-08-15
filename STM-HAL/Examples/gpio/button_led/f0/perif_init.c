#include "button_led.h"

void Peripheral_Init() {

    // Enable GPIO Clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Initialize GPIO Led Pin
    GPIO_InitTypeDef GPIO_InitStruct_Led = {
        .Pin   = GPIO_PIN_3,             // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct_Led);
    
    // Initialize GPIO Button Pin
    GPIO_InitTypeDef GPIO_InitStruct_Button = {
        .Pin   = GPIO_PIN_12,            // pin number
        .Mode  = GPIO_MODE_INPUT,        // input mode
        .Pull  = GPIO_PULLUP,            // enable pull up registers
    };
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct_Button);
}
