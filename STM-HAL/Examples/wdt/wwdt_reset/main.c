#include "wwdt_reset.h"

// Set UART 2 interface as input/output stream
int __io_putchar(int ch) {
  HAL_UART_Transmit(&STDIO_UART, (uint8_t *)&ch, 1, HAL_MAX_DELAY); 
  return ch;
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

    printf("Wait 1 second and refresh WWDT\n");
    HAL_Delay(1000);
    HAL_WWDG_Refresh(&EXAMPLE_WWDG);

    printf("Wait another 1 second and refresh WWDT again\n");
    HAL_Delay(1000);
    HAL_WWDG_Refresh(&EXAMPLE_WWDG);

    printf("Wait 300 msec and refresh WWDT to early\n");
    HAL_Delay(300);
    HAL_WWDG_Refresh(&EXAMPLE_WWDG);

    printf("Never get here\n");

    while (1) {}

}

