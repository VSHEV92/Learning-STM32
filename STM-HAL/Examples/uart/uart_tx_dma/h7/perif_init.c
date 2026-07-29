#include "uart_tx_dma.h"

// UART3 Handler
UART_HandleTypeDef huart3;

// DMA Handler for UART3 TX channel
DMA_HandleTypeDef hdma_usart3_tx;

void USART3_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart3);
}

void DMA1_Stream0_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_usart3_tx);
}

void Peripheral_Init() {
    
    // Enable clocks
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();


    // Initialize GPIO Led Pin
    GPIO_InitTypeDef GPIO_Init_Led = {
        .Pin   = GPIO_PIN_0,             // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(GPIOB, &GPIO_Init_Led);

    // Initialize UART Pin
    GPIO_InitTypeDef GPIO_Init_Uart = {
        .Pin       = GPIO_PIN_8|GPIO_PIN_9,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_VERY_HIGH,  // set slew rate 
        .Alternate = GPIO_AF7_USART3,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOD, &GPIO_Init_Uart);


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


    // Initialize DMA
    hdma_usart3_tx.Instance = DMA1_Stream0;                         // DMA Stream
    hdma_usart3_tx.Init.Request = DMA_REQUEST_USART3_TX;            // DMA request source
    hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;           // DMA transfer direction
    hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;               // don't increment destination address (UART DATA)
    hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;                   // increment source address (memory buffer)
    hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  // set byte width for destination write
    hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     // set byte width for source read
    hdma_usart3_tx.Init.Mode = DMA_NORMAL;                          // one shot transfer, don't cyclic tranfer 
    hdma_usart3_tx.Init.Priority = DMA_PRIORITY_LOW;                // stream priority
    hdma_usart3_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;            // disable fifo mode
    HAL_DMA_Init(&hdma_usart3_tx);


    // Connect UASRT2 and DMA
    //__HAL_LINKDMA(&huart3, hdmatx, hdma_usart3_tx); 
    huart3.hdmatx = &hdma_usart3_tx;
    hdma_usart3_tx.Parent = &huart3;


    // Configure interrupts to maximum priority (lowest value)
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);

    HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

}
