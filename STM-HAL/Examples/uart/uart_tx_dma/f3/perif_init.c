#include "uart_tx_dma.h"

// UART1 Handler
UART_HandleTypeDef huart1;

// DMA Handler for UART1 TX channel
DMA_HandleTypeDef hdma_usart1_tx;


void USART1_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart1);
}

void DMA1_Channel4_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_usart1_tx);
}


void Peripheral_Init() {

    // Enable Clocks
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

    // Initialize GPIO Led Pin
    GPIO_InitTypeDef GPIO_Init_Led = {
        .Pin   = GPIO_PIN_15,            // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(GPIOE, &GPIO_Init_Led);


    // Setup UART GPIO pins
    GPIO_InitTypeDef GPIO_Init_Uart = {
        .Pin       = GPIO_PIN_4|GPIO_PIN_5,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_HIGH,       // set slew rate 
        .Alternate = GPIO_AF7_USART1,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOC, &GPIO_Init_Uart);


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


    // Initialize DMA
    hdma_usart1_tx.Instance = DMA1_Channel4;                        // DMA Channel
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;           // DMA transfer direction
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;               // don't increment destination address (UART DATA)
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;                   // increment source address (memory buffer)
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  // set byte width for destination write
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     // set byte width for source read
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;                          // one shot transfer, don't cyclic tranfer 
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;                // stream priority
    HAL_DMA_Init(&hdma_usart1_tx);


    // Connect UASRT2 and DMA
    //__HAL_LINKDMA(&huart1, hdmatx, hdma_usart1_tx); 
    huart1.hdmatx = &hdma_usart1_tx;
    hdma_usart1_tx.Parent = &huart1;

    // Configure interrupts
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

}

