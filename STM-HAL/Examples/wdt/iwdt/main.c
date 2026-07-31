#include "iwdt.h"

// Set UART interface as input/output stream
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


    // Start IWDT example
    printf("Applications started!\n");
    printf("IWDT enabled\n");

    printf("Wait 0.5 seconds and feed watchdog\n");
    HAL_Delay(500);
    HAL_IWDG_Refresh(&EXAMPLE_IWDG);

    printf("Wait another 0.5 seconds and feed watchdog again\n");
    HAL_Delay(500);
    HAL_IWDG_Refresh(&EXAMPLE_IWDG);

    printf("Wait for reset\n");
    HAL_Delay(3000);

    printf("Never get here\n");

    while (1) {}
}

