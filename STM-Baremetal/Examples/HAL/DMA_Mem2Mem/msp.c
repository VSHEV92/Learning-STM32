#include <stm32f0xx.h>

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_mem2mem;


void HAL_MspInit(void) {

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


    __HAL_RCC_DMA1_CLK_ENABLE();

    hdma_mem2mem.Instance = DMA1_Channel1;
    hdma_mem2mem.Init.Direction = DMA_MEMORY_TO_MEMORY;
    hdma_mem2mem.Init.PeriphInc = DMA_PINC_ENABLE;
    hdma_mem2mem.Init.MemInc = DMA_MINC_ENABLE;
    hdma_mem2mem.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_mem2mem.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_mem2mem.Init.Mode = DMA_NORMAL;
    hdma_mem2mem.Init.Priority = DMA_PRIORITY_LOW;
    HAL_DMA_Init(&hdma_mem2mem);
}


void HAL_UART_MspInit(UART_HandleTypeDef *huart) {

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_Init = {
        .Pin       = GPIO_PIN_2|GPIO_PIN_15,     // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_HIGH,       // set slew rate 
        .Alternate = GPIO_AF1_USART2,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOA, &GPIO_Init);

}
