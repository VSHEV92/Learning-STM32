#include "wwdt_interrupt.h"


// Set UART 2 interface as input/output stream
int __io_putchar(int ch) {
  HAL_UART_Transmit(&STDIO_UART, (uint8_t *)&ch, 1, HAL_MAX_DELAY); 
  return ch;
}

// WWDT HAndler
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg) {
	HAL_WWDG_Refresh(hwwdg);
	printf("Refresh WWDT from ISR\n");
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


    printf("Application started!\n");
    printf("WWDT enabled. Period 2 seconds, window 1 second\n");
    while (1) {}

}

