#include "counter_7seg.h"


// Data for ISR porcessing
uint32_t segment = 0;
uint32_t counter = 0;

unsigned char data_7seg[4] = { ~(0x3F), ~(0x3F), ~(0x3F), ~(0x3F) };

// Digit to 7-segment code convertion 
unsigned char digit_to_7seg(char digit) {
	switch(digit) {
	  case '0': return ~(0x3F);
	  case '1': return ~(0x06);
	  case '2': return ~(0x5b);
	  case '3': return ~(0x4F);
	  case '4': return ~(0x66);
	  case '5': return ~(0x6d);
	  case '6': return ~(0x7d);
	  case '7': return ~(0x07);
	  case '8': return ~(0x7F);
	  case '9': return ~(0x6F);
	}
	return 0;
}

// Setup timer ISR
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    // Update counter value 
    if (htim == &DISPLAY_UPDATE_TIMER) {
	    shiftRegisters[0] = (1 << segment);
	    shiftRegisters[1] = data_7seg[segment];
	
        ShiftRegister74HC595_update();

	    segment = (segment + 1) % 4;
    }

    // Increment Counter Value
    if (htim == &COUNTER_INCREMENT_TIMER) {
	    counter = (counter + 1) % 10000;

        char counter_string[5];
        snprintf(counter_string, sizeof(counter_string), "%04u", (unsigned int)counter);

        for (int i = 0; i < 4; i++) {
    	    data_7seg[i] = digit_to_7seg(counter_string[i]);
        }
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


    // Start timers in interrupt mode 
    HAL_TIM_Base_Start_IT(&DISPLAY_UPDATE_TIMER);
    HAL_TIM_Base_Start_IT(&COUNTER_INCREMENT_TIMER);

    while (1) {}
}

