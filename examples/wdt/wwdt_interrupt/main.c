#include "wwdt_interrupt.h"

UART_HandleTypeDef huart2;
WWDG_HandleTypeDef hwwdg;

// Set UART 2 interface as input/output stream
int __io_putchar(int ch) {
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY); 
  return ch;
}

// WWDT HAndler
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg) {
	HAL_WWDG_Refresh(hwwdg);
	printf("Refresh WWDT from ISR\n");
}

void WWDG_IRQHandler(void) {
    HAL_WWDG_IRQHandler(&hwwdg);
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


    // Set APB1 Clock to 1 MHz
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {
        .ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1,
        .APB1CLKDivider = RCC_HCLK_DIV16,
    };
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);


    // Enable clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_WWDG_CLK_ENABLE();


    // Setup GPIO pins
    GPIO_InitTypeDef GPIO_Init = {
        .Pin       = GPIO_PIN_2|GPIO_PIN_3,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_VERY_HIGH,  // set slew rate 
        .Alternate = GPIO_AF7_USART2,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOA, &GPIO_Init);


    // Initalize UART
    huart2.Instance          = USART2;                 // uart interface instance
    huart2.Init.BaudRate     = 9600;                   // buad rate
    huart2.Init.WordLength   = UART_WORDLENGTH_8B;     // bits per symbol
    huart2.Init.StopBits     = UART_STOPBITS_1;        // 1 stop bit
    huart2.Init.Parity       = UART_PARITY_NONE;       // don't use parity bit
    huart2.Init.Mode         = UART_MODE_TX;           // enable uart tx 
    huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;    // don't use hardware flow control
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;   // set oversampling to 16 bits  
    HAL_UART_Init(&huart2);


    // Initalize WWDT
    hwwdg.Instance       = WWDG;              // WWDT Instance
    hwwdg.Init.Prescaler = WWDG_PRESCALER_8;  // Clock prescaler
    hwwdg.Init.Window    = 96;                // Widnow value (on half of full period)
    hwwdg.Init.Counter   = 127;               // WWDT reload value
    hwwdg.Init.EWIMode   = WWDG_EWI_ENABLE;   // Enable interrupts
    HAL_WWDG_Init(&hwwdg);


    // Enable WWDT interrupts
    HAL_NVIC_SetPriority(WWDG_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(WWDG_IRQn);


    printf("Application started!\n");
    printf("WWDT enabled. Period 2 seconds, window 1 second\n");
    while (1) {}

}

