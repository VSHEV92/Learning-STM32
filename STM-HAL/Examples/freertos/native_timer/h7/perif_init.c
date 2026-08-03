#include "native_timer.h"

// UART3 Handler
UART_HandleTypeDef huart3;

void Peripheral_Init() {
    
    // Enable GPIO Port B,D and UART 3 clocks
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();


    // Setup GPIO pins
    GPIO_InitTypeDef GPIO_Init_UART = {
        .Pin       = GPIO_PIN_8|GPIO_PIN_9,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_VERY_HIGH,  // set slew rate 
        .Alternate = GPIO_AF7_USART3,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOD, &GPIO_Init_UART);

    GPIO_InitTypeDef GPIO_Init_LED = {
        .Pin   = GPIO_PIN_0,             // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(GPIOB, &GPIO_Init_LED);


    // Initalize UART
    huart3.Instance          = USART3;                 // uart interface instance
    huart3.Init.BaudRate     = 9600;                   // buad rate
    huart3.Init.WordLength   = UART_WORDLENGTH_8B;     // bits per symbol
    huart3.Init.StopBits     = UART_STOPBITS_1;        // 1 stop bit
    huart3.Init.Parity       = UART_PARITY_NONE;       // don't use parity bit
    huart3.Init.Mode         = UART_MODE_TX;           // enable uart tx 
    huart3.Init.HwFlowCtl    = UART_HWCONTROL_NONE;    // don't use hardware flow control
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;   // set oversampling to 16 bits  
    HAL_UART_Init(&huart3);

}
