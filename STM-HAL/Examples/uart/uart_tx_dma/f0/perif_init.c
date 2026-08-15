#include "uart_tx_dma.h"

// UART2 Handler
UART_HandleTypeDef huart2;

// DMA Handler for UART2 TX channel
DMA_HandleTypeDef hdma_usart2_tx;


void USART2_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart2);
}

void DMA1_Channel4_5_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_usart2_tx);
}

void Peripheral_Init() {
    
    // Enable clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();


    // Setup GPIO pins
    GPIO_InitTypeDef GPIO_Init_Led = {
        .Pin   = GPIO_PIN_3,             // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(GPIOB, &GPIO_Init_Led);

    GPIO_InitTypeDef GPIO_Init = {
        .Pin       = GPIO_PIN_2|GPIO_PIN_15,     // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_HIGH,       // set slew rate 
        .Alternate = GPIO_AF1_USART2,            // choose UART2 as alternative function
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


    // Initialize DMA
    hdma_usart2_tx.Instance = DMA1_Channel4;                        // DMA instance
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;           // DMA transfer direction
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;               // don't increment destination address (UART DATA)
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;                   // increment source address (memory buffer)
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  // set byte width for destination write
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     // set byte width for source read
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;                          // one shot transfer, don't cyclic tranfer 
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;                // stream priority
    HAL_DMA_Init(&hdma_usart2_tx);


    // Connect UASRT2 and DMA
    //__HAL_LINKDMA(&huart2, hdmatx, hdma_usart2_tx); 
    huart2.hdmatx = &hdma_usart2_tx;
    hdma_usart2_tx.Parent = &huart2;

    // Configure interrupts
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);

    HAL_NVIC_SetPriority(DMA1_Channel4_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);

}

