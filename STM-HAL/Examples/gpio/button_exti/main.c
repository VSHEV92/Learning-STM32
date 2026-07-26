#include "button_exti.h"

// external interrupt ISR
void EXTI_ISR(void) {
    HAL_GPIO_EXTI_IRQHandler(BUTTON_PIN);
}

// external interrupt Callback
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == BUTTON_PIN) {
	    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
    }
}


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


    while (1) {}
}

