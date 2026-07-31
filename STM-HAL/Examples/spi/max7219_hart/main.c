#include "max7219_hart.h"

// Display Data
uint8_t max7219_data[] = {
    0b00000000,
    0b01101100,
    0b11111110,
    0b11111110,
    0b01111100,
    0b00111000,
    0b00010000,
    0b00000000
};


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


    // Initialize MAX7219 driver 
    max7219_Init();
    max7219_SendData(REG_DISPLAY_TEST, 0x0);
    
    for (int i = 0; i < 8; i++) {
	    max7219_SendData(i+1, max7219_data[i]);
    }

    uint8_t intensity = 0;
    max7219_SetIntensivity(intensity);
    max7219_TurnOn();
    
    while (1) {
        HAL_Delay(1500/16);
	    intensity = (intensity + 1) % 16;
	    max7219_SetIntensivity(intensity);
    }
}

