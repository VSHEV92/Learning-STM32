#include "adc_timer.h"

// UART3 Handler
UART_HandleTypeDef huart3;
ADC_HandleTypeDef hadc1;
TIM_HandleTypeDef htim2;


void ADC_IRQHandler(void) {
    HAL_ADC_IRQHandler(&hadc1);
}

void Peripheral_Init() {
    
    // Enable GPIO Port D and UART 3 clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();
    __HAL_RCC_ADC12_CLK_ENABLE();
    __HAL_RCC_TIM2_CLK_ENABLE();


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


    // ------------------------------------------------
    // --------------- Setup GPIOs --------------------
    // ------------------------------------------------

    // Setup GPIO pins
    GPIO_InitTypeDef GPIO_Init_Uart = {
        .Pin       = GPIO_PIN_8|GPIO_PIN_9,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_VERY_HIGH,  // set slew rate 
        .Alternate = GPIO_AF7_USART3,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOD, &GPIO_Init_Uart);

    GPIO_InitTypeDef GPIO_Init_ADC = {
        .Pin       = GPIO_PIN_3,       // choose GPIO pins
        .Mode      = GPIO_MODE_ANALOG, // set pin to analog mode
        .Pull      = GPIO_NOPULL,      // disable pull up/down registers
    };
    HAL_GPIO_Init(GPIOA, &GPIO_Init_ADC);



    // ------------------------------------------------
    // --------------- Setup UART ---------------------
    // ------------------------------------------------

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


    // ------------------------------------------------
    // --------------- Setup ADC ----------------------
    // ------------------------------------------------

    // Initialize ADC
    hadc1.Instance                   = ADC1;                            // ADC instance
    hadc1.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV4;            // Set ADC Clock as one fourth of PCLK clock
    hadc1.Init.Resolution            = ADC_RESOLUTION_16B;              // Set convertion resolution to 12 bit
    hadc1.Init.ScanConvMode          = ADC_SCAN_DISABLE;                // don't use scan mode (only one channel converted)
    hadc1.Init.ContinuousConvMode    = DISABLE;                         // don't use continuous mode (one shot)
    hadc1.Init.NbrOfConversion       = 1;                               // one convertion in convertion sequence
    hadc1.Init.ExternalTrigConv      = ADC_SOFTWARE_START;              // run convertion by sofwtare call
    hadc1.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;             // interrut on end of convertion
    hadc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING; // use external trigger rising edge
    hadc1.Init.ExternalTrigConv      = ADC_EXTERNALTRIG_T2_TRGO;        // run from external trigger from Timer 2
    HAL_ADC_Init(&hadc1);

    ADC_ChannelConfTypeDef sConfig = {
        .Channel      = ADC_CHANNEL_15,             // Set ADC channel
        .Rank         = ADC_REGULAR_RANK_1,         // Set sequence number for channel
        .SamplingTime = ADC_SAMPLETIME_1CYCLE_5,    // Sampling time
        .SingleDiff   = ADC_SINGLE_ENDED,           // Single ended input for ADC   
    };
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);


    // Enable ADC interrupts
    HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);


    // ------------------------------------------------
    // --------------- Setup Timer --------------------
    // ------------------------------------------------

    htim2.Instance               = TIM2;                            // set timer instance  
    htim2.Init.Prescaler         = 64000 - 1;                       // set timer clock prescaler
    htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;              // set counter to UP mode 
    htim2.Init.Period            = 333 - 1;                         // set timer period
    htim2.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;          // set timer clock division of sampling clock 
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;  // set disable shadow preload register 
    HAL_TIM_Base_Init(&htim2);

    TIM_MasterConfigTypeDef TRO_Config = {
        .MasterOutputTrigger = TIM_TRGO_UPDATE,              // send output trigger on timer overflow
    };
    HAL_TIMEx_MasterConfigSynchronization(&htim2, &TRO_Config);
}
