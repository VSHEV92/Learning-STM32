#include "pwm_led.h"

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


    // Start timer in PWM mode 
    HAL_TIM_PWM_Start(&EXEMPLE_TIMER, EXEMPLE_TIMER_PWM_CHANNEL);

    // Update PWM compare value
    uint32_t pulse_width = 0;
    while (1) {
	    __HAL_TIM_SET_COMPARE(&EXEMPLE_TIMER, EXEMPLE_TIMER_PWM_CHANNEL, pulse_width++);
        pulse_width %= 100;
	    HAL_Delay(10);
    }
}

