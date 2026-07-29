#include "base_dma.h"


// Send half data to timer period register
void HAL_TIM_PeriodElapsedHalfCpltCallback(TIM_HandleTypeDef *htim) {
	HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
}

// Send all data to timer period register
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
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


    // Timer update periods
    uint16_t periods[2] = {
        200  - 1,
	    1000  - 1
    };

    // Start timer in DMA mode 
    HAL_TIM_Base_Start_DMA(&EXEMPLE_TIMER, (const uint32_t*)periods, 2);

    while (1) {}
}

