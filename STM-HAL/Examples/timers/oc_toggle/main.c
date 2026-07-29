#include "oc_toggle.h"

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


    // Start timer in output capture mode 
    HAL_TIM_OC_Start(&EXEMPLE_TIMER, EXEMPLE_TIMER_OC_CHANNEL);

    while (1) {}
}

