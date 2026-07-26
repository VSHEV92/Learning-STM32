#include "gpio_7seg.h"


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


    // Data fot 7-segments led (BEAF)
    unsigned char data_7seg[4][2] = {
          { 0x01, ~(0x7F) },
          { 0x02, ~(0x79) },
          { 0x04, ~(0x77) },
          { 0x08, ~(0x71) },
    };

    while (1) {
        for(int i = 0; i < 4; i++) {
            shiftRegisters[0] = data_7seg[i][0];
            shiftRegisters[1] = data_7seg[i][1];
            ShiftRegister74HC595_update();
        }
    }
}

