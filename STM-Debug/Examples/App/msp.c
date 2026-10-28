#include <stm32f0xx.h>

UART_HandleTypeDef huart2;
TIM_HandleTypeDef  htim14;

void HAL_MspInit(void) {

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin   = GPIO_PIN_5,             // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


    __HAL_RCC_USART2_CLK_ENABLE();
    
    huart2.Instance          = USART2;                 // uart interface instance
    huart2.Init.BaudRate     = 9600;                   // buad rate
    huart2.Init.WordLength   = UART_WORDLENGTH_8B;     // bits per symbol
    huart2.Init.StopBits     = UART_STOPBITS_1;        // 1 stop bit
    huart2.Init.Parity       = UART_PARITY_NONE;       // don't use parity bit
    huart2.Init.Mode         = UART_MODE_TX;           // enable uart tx 
    huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;    // don't use hardware flow control
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;   // set oversampling to 16 bits  
    HAL_UART_Init(&huart2);


    __HAL_RCC_TIM14_CLK_ENABLE();

    htim14.Instance = TIM14;
    htim14.Init.Period = 8000;
    htim14.Init.Prescaler = 3000;
    htim14.Init.CounterMode = TIM_COUNTERMODE_UP;

    HAL_TIM_Base_Init(&htim14);
    HAL_TIM_Base_Start_IT(&htim14);

    HAL_NVIC_EnableIRQ(TIM14_IRQn);

}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_Init = {
        .Pin       = GPIO_PIN_2|GPIO_PIN_3,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_HIGH,       // set slew rate 
        .Alternate = GPIO_AF1_USART2,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOA, &GPIO_Init);

}

// Redirect stdlib stream to UART
int _write(int file, char *ptr, int len)  {
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}
