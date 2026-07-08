#include "uart_tx_it.h"

// UART2 Handler
UART_HandleTypeDef huart2;

// Set Uart TX completion callback to led toggling 
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

void USART2_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart2);
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

    
    // Enable GPIO Port A and UART 2 clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    // Initialize GPIO Led Pin
    GPIO_InitTypeDef GPIO_Init_Led = {
        .Pin   = GPIO_PIN_5,             // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(GPIOA, &GPIO_Init_Led);


    // Setup UART GPIO pins
    GPIO_InitTypeDef GPIO_Init_Uart = {
        .Pin       = GPIO_PIN_2|GPIO_PIN_3,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_VERY_HIGH,  // set slew rate 
        .Alternate = GPIO_AF7_USART2,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOA, &GPIO_Init_Uart);


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

    // Configure interrupts to maximum priority (lowest value)
    HAL_NVIC_SetPriority(
        USART2_IRQn, // IRQ number
        0,          // Preempt Priority
        0           // Sub Priority
    );
    HAL_NVIC_EnableIRQ(USART2_IRQn);

    // print counter values
    uint8_t cntr = 0;
    char msg[30];
    
    while (1) {
        sprintf(msg, "Counter value: %3d\n", cntr);
        cntr++;

        HAL_UART_Transmit_IT(&huart2, (uint8_t*)msg, strlen(msg));
        HAL_Delay(300);
    }
}

