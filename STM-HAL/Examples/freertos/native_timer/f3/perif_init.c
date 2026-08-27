#include "native_timer.h"

UART_HandleTypeDef huart1;

void Peripheral_Init() {

    // Enable clocks
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();


    // Setup GPIO pins
    GPIO_InitTypeDef GPIO_Init_UART = {
        .Pin       = GPIO_PIN_4|GPIO_PIN_5,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_HIGH,       // set slew rate 
        .Alternate = GPIO_AF7_USART1,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOC, &GPIO_Init_UART);

    GPIO_InitTypeDef GPIO_Init_LED = {
        .Pin   = GPIO_PIN_15,             // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(GPIOE, &GPIO_Init_LED);


    // Initalize UART
    huart1.Instance          = USART1;                 // uart interface instance
    huart1.Init.BaudRate     = 9600;                   // buad rate
    huart1.Init.WordLength   = UART_WORDLENGTH_8B;     // bits per symbol
    huart1.Init.StopBits     = UART_STOPBITS_1;        // 1 stop bit
    huart1.Init.Parity       = UART_PARITY_NONE;       // don't use parity bit
    huart1.Init.Mode         = UART_MODE_TX;           // enable uart tx 
    huart1.Init.HwFlowCtl    = UART_HWCONTROL_NONE;    // don't use hardware flow control
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;   // set oversampling to 16 bits  
    HAL_UART_Init(&huart1);


    // PendSV_IRQn interrupt configuration
    HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);

}



