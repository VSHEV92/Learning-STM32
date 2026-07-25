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

    /*
     *  Initialize example peripheral
     */
    Peripheral_Init();


    while (1) {
	    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
	    HAL_Delay(TOGGLE_DELAY);
    }
}

