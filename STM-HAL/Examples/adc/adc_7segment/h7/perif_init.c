#include "adc_7segment.h"

// Peripheral Handler
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;


void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim2);
}

void TIM3_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim3);
}

void DMA1_Stream0_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_adc1);
}

void Peripheral_Init() {
    
    // Enable clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_TIM3_CLK_ENABLE();

    __HAL_RCC_ADC12_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();


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

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin = SERIAL_CLK_Pin;
    HAL_GPIO_Init(SERIAL_CLK_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SERIAL_DATA_Pin;
    HAL_GPIO_Init(SERIAL_DATA_GPIO_Port, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = LATCH_CLK_Pin;
    HAL_GPIO_Init(LATCH_CLK_GPIO_Port, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


    // ------------------------------------------------
    // --------------- Setup ADC ----------------------
    // ------------------------------------------------

    // Initialize ADC
    hadc1.Instance                      = ADC1;                            // ADC instance
    hadc1.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV4;            // Set ADC Clock as one fourth of PCLK clock
    hadc1.Init.Resolution               = ADC_RESOLUTION_16B;              // Set convertion resolution to 12 bit
    hadc1.Init.ScanConvMode             = ADC_SCAN_DISABLE;                // don't use scan mode (only one channel converted)
    hadc1.Init.ContinuousConvMode       = ENABLE;                          // use continuous mode (repeated convertions)
    hadc1.Init.NbrOfConversion          = 1;                               // one convertion in convertion sequence
    hadc1.Init.ExternalTrigConv         = ADC_SOFTWARE_START;              // run convertion by sofwtare call
    hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR; // Enable DMA mode
    hadc1.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;             // interrut on end of convertion
    HAL_ADC_Init(&hadc1);

    ADC_ChannelConfTypeDef sConfig = {
        .Channel      = ADC_CHANNEL_15,             // Set ADC channel
        .Rank         = ADC_REGULAR_RANK_1,         // set sequence number for channel
        .SamplingTime = ADC_SAMPLETIME_1CYCLE_5,    // Sampling time
        .SingleDiff   = ADC_SINGLE_ENDED,           // Single ended input for ADC   
    };
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);


    // ------------------------------------------------
    // --------------- Setup DMA ----------------------
    // ------------------------------------------------
    hdma_adc1.Instance = DMA1_Stream0;
    hdma_adc1.Init.Request = DMA_REQUEST_ADC1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_DISABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
    hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_adc1);

    __HAL_LINKDMA(&hadc1,DMA_Handle,hdma_adc1);

    HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

    // ------------------------------------------------
    // --------------- Setup Timer --------------------
    // ------------------------------------------------

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 6400 - 1;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 10;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&htim2);

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 64000 - 1;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 333;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&htim3);

    // Setup timers interrupts
    HAL_NVIC_SetPriority( TIM2_IRQn, 0, 0 );
    HAL_NVIC_EnableIRQ(TIM2_IRQn );

    HAL_NVIC_SetPriority( TIM3_IRQn, 0, 0 );
    HAL_NVIC_EnableIRQ(TIM3_IRQn );
}
