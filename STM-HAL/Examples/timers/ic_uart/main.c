#include "ic_uart.h"

// putchar redifinition for printf
int __io_putchar(int ch) {
    HAL_UART_Transmit(&STDIO_UART, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

// Setup timer ISR
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    // Get input capture value
    uint32_t captured_value = HAL_TIM_ReadCapturedValue(htim, EXEMPLE_TIMER_IC_CHANNEL);
    // Send this value to UART
    print_value(captured_value);
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


    // Start timer with input capture mode on needed channel
    HAL_TIM_IC_Start_IT(&EXEMPLE_TIMER, EXEMPLE_TIMER_IC_CHANNEL);

    while (1) {}
}

