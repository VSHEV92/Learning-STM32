#include <stm32f0xx.h>
#include <main.h>


UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_tx;


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

    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
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


    __HAL_RCC_DMA1_CLK_ENABLE();

    hdma_usart2_tx.Instance = DMA1_Channel4;                        // DMA instance
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;           // DMA transfer direction
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;               // don't increment destination address (UART DATA)
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;                   // increment source address (memory buffer)
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  // set byte width for destination write
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     // set byte width for source read
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;                          // one shot transfer, don't cyclic tranfer 
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;                // stream priority
    HAL_DMA_Init(&hdma_usart2_tx);
    
    __HAL_LINKDMA(huart, hdmatx, hdma_usart2_tx); 

    HAL_NVIC_SetPriority(DMA1_Channel4_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);
}
