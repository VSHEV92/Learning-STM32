#include "eeprom_mem_dma.h"

// Peripheral Handlers
UART_HandleTypeDef huart1;
I2C_HandleTypeDef hi2c1;

DMA_HandleTypeDef hdma_i2c1_rx;
DMA_HandleTypeDef hdma_i2c1_tx;

void I2C1_EV_IRQHandler(void) {
    HAL_I2C_EV_IRQHandler(&hi2c1);
}

// DMA ISR Handlers
void DMA1_Channel6_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_i2c1_tx);
}

void DMA1_Channel7_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_i2c1_rx);
}

void Peripheral_Init() {
    
    // Enable clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_I2C1_CLK_ENABLE();

    __HAL_RCC_DMA1_CLK_ENABLE();


    // Setup GPIO pins
    GPIO_InitTypeDef GPIO_Init_UART = {
        .Pin       = GPIO_PIN_4|GPIO_PIN_5,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_HIGH,       // set slew rate 
        .Alternate = GPIO_AF7_USART1,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOC, &GPIO_Init_UART);

    GPIO_InitTypeDef GPIO_Init_I2C = {
        .Mode      = GPIO_MODE_AF_OD,            // set pins mode to alternative function with open drain
        .Pull      = GPIO_PULLUP,                // enable pull up registers
        .Speed     = GPIO_SPEED_FREQ_LOW,        // set slew rate 
        .Alternate = GPIO_AF4_I2C1,              // choose I2C1 as alternative function
    };

    GPIO_Init_I2C.Pin = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOB, &GPIO_Init_I2C);

    GPIO_Init_I2C.Pin = GPIO_PIN_15;
    HAL_GPIO_Init(GPIOA, &GPIO_Init_I2C);

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

    // Initialize I2C
    hi2c1.Instance             = I2C1;                    // I2C instance 
    hi2c1.Init.Timing          = 0x00201D2B;
    hi2c1.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT; // Address mode is 7 bits
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; // Disable dual address mode
    hi2c1.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;   // Disable clock streching
    HAL_I2C_Init(&hi2c1);


    // Initialize RX DMA Stream
    hdma_i2c1_rx.Instance = DMA1_Channel7;
    hdma_i2c1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_i2c1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c1_rx.Init.Mode = DMA_NORMAL;
    hdma_i2c1_rx.Init.Priority = DMA_PRIORITY_LOW;
    HAL_DMA_Init(&hdma_i2c1_rx);

    __HAL_LINKDMA(&hi2c1,hdmarx,hdma_i2c1_rx);


    // Initialize TX DMA Stream
    hdma_i2c1_tx.Instance = DMA1_Channel6;
    hdma_i2c1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_i2c1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c1_tx.Init.Mode = DMA_NORMAL;
    hdma_i2c1_tx.Init.Priority = DMA_PRIORITY_LOW;
    HAL_DMA_Init(&hdma_i2c1_tx);

    __HAL_LINKDMA(&hi2c1,hdmatx,hdma_i2c1_tx);

    
    // Enable interrupts
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);

    HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

    HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
}

