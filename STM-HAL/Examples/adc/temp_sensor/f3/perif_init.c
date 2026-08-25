#include "temp_sensor.h"

// Peripheral Handler
UART_HandleTypeDef huart1;
ADC_HandleTypeDef hadc1;


void Peripheral_Init() {

    // Enable Clock
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_ADC1_CLK_ENABLE();


    // Setup GPIO pins
    GPIO_InitTypeDef GPIO_Init = {
        .Pin       = GPIO_PIN_4|GPIO_PIN_5,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_HIGH,       // set slew rate 
        .Alternate = GPIO_AF7_USART1,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOC, &GPIO_Init);


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


    // Initialize ADC
    hadc1.Instance                   = ADC1;                          // ADC instance
    hadc1.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV2;      // Set ADC Clock as one half of PCLK clock
    hadc1.Init.Resolution            = ADC_RESOLUTION_12B;            // set convertion resolution to 12 bit
    hadc1.Init.ScanConvMode          = DISABLE;                       // don't use scan mode (only one channel converted)
    hadc1.Init.ContinuousConvMode    = DISABLE;                       // don't use continuous mode (one shot)
    hadc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; // don't use external trigger for convertion
    hadc1.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            // run convertion by sofwtare call
    hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;           // right align of 12 bit data in register
    hadc1.Init.NbrOfConversion       = 1;                             // one convertion in convertion sequence
    hadc1.Init.DMAContinuousRequests = DISABLE;                       // disable DMA ode
    hadc1.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;           // interrut on end of convertion
    HAL_ADC_Init(&hadc1);

    // Configure ADC Channel
    ADC_ChannelConfTypeDef Channel_Config = {
        .Channel      = ADC_CHANNEL_TEMPSENSOR, // use temperature sensor channel
        .Rank         = 1,                      // set sequence number for channel
        .SamplingTime = ADC_SAMPLETIME_7CYCLES_5, // set 3 ACLK cycle for sampling
    };
    HAL_ADC_ConfigChannel(&hadc1, &Channel_Config);

}



