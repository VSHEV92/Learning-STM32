#include "led_toggle.h"

#define TOGGLE_DELAY 100

void main() {

    /*
     * 1. Enable instruction and data cache. Enable instruction prefetch.
     * 2. Set NVIC priority grouping to 4 bits 
     * 3. Call HAL_InitTick(), which:
     *    - set Systick to 1ms period
     *    - set Systick IRQ priority to TICK_INT_PRIORITY (minimum).
     */
    HAL_Init();

    // Enable GPIO clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Initialize GPIO pin
    GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin   = LED_PIN,                // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
    

    while (1) {
	    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
	    HAL_Delay(TOGGLE_DELAY);
    }
}

