#include "temp_sensor.h"

// UART3 Handler
UART_HandleTypeDef huart3;
ADC_HandleTypeDef hadc3;

void Peripheral_Init() {
    
    // Enable GPIO Port D and UART 3 clocks
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();
    __HAL_RCC_ADC3_CLK_ENABLE();


    // Configure ADC clock
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInitStruct.PLL2.PLL2M = 32;
    PeriphClkInitStruct.PLL2.PLL2N = 129;
    PeriphClkInitStruct.PLL2.PLL2P = 2;
    PeriphClkInitStruct.PLL2.PLL2Q = 2;
    PeriphClkInitStruct.PLL2.PLL2R = 2;
    PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_1;
    PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
    PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
    PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);


    // Setup GPIO pins
    GPIO_InitTypeDef GPIO_Init = {
        .Pin       = GPIO_PIN_8|GPIO_PIN_9,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_VERY_HIGH,  // set slew rate 
        .Alternate = GPIO_AF7_USART3,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOD, &GPIO_Init);


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

    // Initialize ADC
    hadc3.Instance                = ADC3;                 // ADC instance
    hadc3.Init.ClockPrescaler     = ADC_CLOCK_ASYNC_DIV4; // Set ADC Clock as one fourth of PCLK clock
    hadc3.Init.Resolution         = ADC_RESOLUTION_16B;   // Set convertion resolution to 12 bit
    hadc3.Init.ScanConvMode       = ADC_SCAN_DISABLE;     // don't use scan mode (only one channel converted)
    hadc3.Init.ContinuousConvMode = DISABLE;              // don't use continuous mode (one shot)
    hadc3.Init.NbrOfConversion    = 1;                    // one convertion in convertion sequence
    hadc3.Init.ExternalTrigConv   = ADC_SOFTWARE_START;   // run convertion by sofwtare call
    hadc3.Init.EOCSelection       = ADC_EOC_SINGLE_CONV;  // interrut on end of convertion
    HAL_ADC_Init(&hadc3);

    ADC_ChannelConfTypeDef sConfig = {
        .Channel      = ADC_CHANNEL_TEMPSENSOR,     // use temperature sensor channel
        .Rank         = ADC_REGULAR_RANK_1,         // set sequence number for channel
        .SamplingTime = ADC_SAMPLETIME_810CYCLES_5, // Sampling time
        .SingleDiff   = ADC_SINGLE_ENDED,           // Single ended input for ADC   
    };
    HAL_ADC_ConfigChannel(&hadc3, &sConfig);
}
