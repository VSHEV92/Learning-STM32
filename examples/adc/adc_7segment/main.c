#include "adc_7segment.h"

// Peripheral Handler
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

// Data for ISR porcessing
uint32_t segment = 0;
uint32_t adc_value = 0;

unsigned char data_7seg[4] = { ~(0x3F), ~(0x3F), ~(0x3F), ~(0x3F) };

// Digit to 7-segment code convertion 
unsigned char digit_to_7seg(char digit) {
	switch(digit) {
	  case '0': return ~(0x3F);
	  case '1': return ~(0x06);
	  case '2': return ~(0x5b);
	  case '3': return ~(0x4F);
	  case '4': return ~(0x66);
	  case '5': return ~(0x6d);
	  case '6': return ~(0x7d);
	  case '7': return ~(0x07);
	  case '8': return ~(0x7F);
	  case '9': return ~(0x6F);
	}
	return 0;
}

// ------------------------------------------------
// ---------------- Setup ISRs --------------------
// ------------------------------------------------

// Setup timer ISR
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim2) {
	    shiftRegisters[0] = (1 << segment);
	    shiftRegisters[1] = data_7seg[segment];
	    ShiftRegister74HC595_update();

	    segment = (segment + 1) % 4;
    }

    if (htim == &htim3) {

        char data_7seg_string[5];

        snprintf(data_7seg_string, sizeof(data_7seg_string), "%04u", (unsigned int)adc_value);

        for (int i = 0; i < 4; i++) {
    	    data_7seg[i] = digit_to_7seg(data_7seg_string[i]);
        }

    }
}

void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim2);
}

void TIM3_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim3);
}

void DMA2_Stream0_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_adc1);
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

    
    // Enable Clock
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_TIM3_CLK_ENABLE();

    __HAL_RCC_ADC1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();

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
    
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


    // ------------------------------------------------
    // --------------- Setup ADC ----------------------
    // ------------------------------------------------

    // Initialize ADC
    hadc1.Instance                   = ADC1;                            // ADC instance
    hadc1.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV2;        // Set ADC Clock as one half of PCLK clock
    hadc1.Init.Resolution            = ADC_RESOLUTION_12B;              // set convertion resolution to 12 bit
    hadc1.Init.ScanConvMode          = DISABLE;                         // don't use scan mode (only one channel converted)
    hadc1.Init.ContinuousConvMode    = ENABLE;                          // use continuous mode (repeated convertions)
    hadc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;   // don't use external trigger for convertion
    hadc1.Init.ExternalTrigConv      = ADC_SOFTWARE_START;              // run convertion by sofwtare call
    hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;             // right align of 12 bit data in register
    hadc1.Init.NbrOfConversion       = 1;                               // one convertion in convertion sequence
    hadc1.Init.DMAContinuousRequests = ENABLE;                          // use DMA mode
    hadc1.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;             // interrut on end of convertion
    HAL_ADC_Init(&hadc1);

    // Configure ADC Channel
    ADC_ChannelConfTypeDef Channel_Config = {
        .Channel      = ADC_CHANNEL_0,          // use ADC channel 0
        .Rank         = 1,                      // set sequence number for channel
        .SamplingTime = ADC_SAMPLETIME_3CYCLES, // set 3 ACLK cycle for sampling
    };
    HAL_ADC_ConfigChannel(&hadc1, &Channel_Config);

    // ------------------------------------------------
    // --------------- Setup DMA ----------------------
    // ------------------------------------------------
    hdma_adc1.Instance = DMA2_Stream0;
    hdma_adc1.Init.Channel = DMA_CHANNEL_0;
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

    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

    // ------------------------------------------------
    // --------------- Setup Timer --------------------
    // ------------------------------------------------

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 1600 - 1;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 10;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&htim2);

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 16000 - 1;
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


    // ------------------------------------------------
    // -------------- Start Peripheral ----------------
    // ------------------------------------------------
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_Base_Start_IT(&htim3);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc_value, 1);

    while (1) {}
}

