#include <stm32f0xx.h>
#include <main.h>

ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_tx;

void HAL_MspInit(void) {

    // ----------------------------------------
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


    // ----------------------------------------
    __HAL_RCC_ADC1_CLK_ENABLE();

    // Initialize ADC
    hadc1.Instance                   = ADC1;                          // ADC instance
    hadc1.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV2;      // Set ADC Clock as one half of PCLK clock
    hadc1.Init.Resolution            = ADC_RESOLUTION_12B;            // set convertion resolution to 12 bit
    hadc1.Init.ScanConvMode          = DISABLE;                       // don't use scan mode (only one channel converted)
    hadc1.Init.ContinuousConvMode    = DISABLE;                       // don't use continuous mode (one shot)
    hadc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; // don't use external trigger for convertion
    hadc1.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            // run convertion by sofwtare call
    hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;           // right align of 12 bit data in register
    hadc1.Init.DMAContinuousRequests = DISABLE;                       // disable DMA ode
    hadc1.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;           // interrut on end of convertion
    HAL_ADC_Init(&hadc1);

    // Configure ADC Channel
    ADC_ChannelConfTypeDef Channel_Config = {
        .Channel      = ADC_CHANNEL_TEMPSENSOR,     // use temperature sensor channel
        .Rank         = 1,                          // set sequence number for channel
        .SamplingTime = ADC_SAMPLETIME_239CYCLES_5  // set 239.5 ACLK cycle for sampling
    };
    HAL_ADC_ConfigChannel(&hadc1, &Channel_Config);

    // Calibrate ADC
    HAL_ADCEx_Calibration_Start(&hadc1);

    // --------------------------------------------------

    // Enable Sleep-On_Exit
    HAL_PWR_EnableSleepOnExit();

    // --------------------------------------------------

    // Enable Interrupts
    HAL_NVIC_EnableIRQ(ADC1_IRQn);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);
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

}
