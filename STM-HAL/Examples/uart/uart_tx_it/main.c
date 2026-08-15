#include "uart_tx_it.h"

// Set Uart TX completion callback to led toggling 
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
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
    

    // print counter values
    uint8_t cntr = 0;

#ifndef STM32F042x6
    char msg[30];
    
    while (1) {
        sprintf(msg, "Counter value: %3d\n", cntr);
        cntr++;

        HAL_UART_Transmit_IT(&STDIO_UART, (uint8_t*)msg, strlen(msg));
        HAL_Delay(300);
    }
#else
    char msg[] = "Counter value: ";
    
    uint8_t digit;
    uint8_t eol = '\n';

    while (1) {
        cntr++;

        HAL_UART_Transmit(&STDIO_UART, (uint8_t*)msg, sizeof(msg)-1, HAL_MAX_DELAY);
        
        // Digits convertion
        
        digit = cntr / 100 + '0'; 
        HAL_UART_Transmit(&STDIO_UART, &digit, 1, HAL_MAX_DELAY);

        digit = cntr % 100; 
        digit = digit / 10 + '0'; 
        HAL_UART_Transmit(&STDIO_UART, &digit, 1, HAL_MAX_DELAY);

        digit = cntr % 10 + '0'; 
        HAL_UART_Transmit(&STDIO_UART, &digit, 1, HAL_MAX_DELAY);

        HAL_UART_Transmit_IT(&STDIO_UART, &eol, 1);
        
        HAL_Delay(300);
    }

#endif
}

