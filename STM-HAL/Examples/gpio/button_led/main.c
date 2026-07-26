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

    /*
     *  Initialize example peripheral
     */
    Peripheral_Init();

    uint32_t button_value;
    while (1) {
	    button_value = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);
	    HAL_GPIO_WritePin(LED_PORT, LED_PIN, button_value);
    }
}

