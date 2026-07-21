#include "uart_tx_dma.h"

// UART2 Handler
UART_HandleTypeDef huart2;

// DMA Handler for UART2 TX channel
DMA_HandleTypeDef hdma_usart2_tx;


// Set Uart TX completion callback to led toggling 
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

void USART2_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart2);
}

void DMA1_Stream6_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_usart2_tx);
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

    
    // Enable Clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

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


    // Initialize DMA
    hdma_usart2_tx.Instance = DMA1_Stream6;                         // DMA Stream
    hdma_usart2_tx.Init.Channel = DMA_CHANNEL_4;                    // DMA channel inside stream
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;           // DMA transfer direction
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;               // don't increment destination address (UART DATA)
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;                   // increment source address (memory buffer)
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  // set byte width for destination write
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     // set byte width for source read
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;                          // one shot transfer, don't cyclic tranfer 
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;                // stream priority
    hdma_usart2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;            // disable fifo mode
    HAL_DMA_Init(&hdma_usart2_tx);


    // Connect UASRT2 and DMA
    //__HAL_LINKDMA(&huart2, hdmatx, hdma_usart2_tx); 
    huart2.hdmatx = &hdma_usart2_tx;
    hdma_usart2_tx.Parent = &huart2;

    // Configure interrupts
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);

    HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);


    // print counter values
    uint8_t cntr = 0;
    char msg[30];
    
    while (1) {
        sprintf(msg, "Counter value: %3d\n", cntr);
        cntr++;

        HAL_UART_Transmit_DMA(&huart2, (uint8_t*)msg, strlen(msg));
        HAL_Delay(500);
    }
}

