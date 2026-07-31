#include "max7219_blink.h"

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
    

    // blink by all LEDs
    while (1) {
	    max7219_SendData(REG_DISPLAY_TEST, 0x1);
	    HAL_Delay(500);

	    max7219_SendData(REG_DISPLAY_TEST, 0x0);
	    HAL_Delay(500);
    }
}

