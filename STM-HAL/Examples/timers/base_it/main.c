#include "base_it.h"

// Setup timer ISR
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &EXEMPLE_TIMER) {
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


    // Start timer in interrupt mode 
    HAL_TIM_Base_Start_IT(&htim1);

    while (1) {}
}

