#include "button_led.h"

void main() {

    /*
     * 1. Enable instruction and data cache. Enable instruction prefetch.
     * 2. Set NVIC priority grouping to 4 bits 
     * 3. Call HAL_InitTick(), which:
     *    - set Systick to 1ms period
     *    - set Systick IRQ priority to TICK_INT_PRIORITY (minimum).
     */
    HAL_Init();

    // Enable GPIO Clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();

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
        .Pin   = GPIO_PIN_1,             // pin number
        .Mode  = GPIO_MODE_INPUT,        // input mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct_Button);

    uint32_t button_value;
    while (1) {
	    button_value = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, button_value);
    }
}

