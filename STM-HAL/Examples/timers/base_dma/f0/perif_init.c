#include "base_dma.h"

TIM_HandleTypeDef htim1;
DMA_HandleTypeDef hdma_tim1;

void TIM1_BRK_UP_TRG_COM_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim1);
}

void DMA1_Channel4_5_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_tim1);
}


void Peripheral_Init() {

    // Enable Clocks
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_TIM1_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();


    // Setup interrupts
    HAL_NVIC_SetPriority( TIM1_BRK_UP_TRG_COM_IRQn, 0, 0 );
    HAL_NVIC_EnableIRQ( TIM1_BRK_UP_TRG_COM_IRQn );

    HAL_NVIC_SetPriority(DMA1_Channel4_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);


    // Initialize GPIO pin
    GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin   = GPIO_PIN_3,             // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


    hdma_tim1.Instance                 = DMA1_Channel5;           // DMA Instance
    hdma_tim1.Init.Direction           = DMA_MEMORY_TO_PERIPH;    // transfer direction Memory to Peripheral
    hdma_tim1.Init.PeriphInc           = DMA_PINC_DISABLE;        // don't increment destination address 
    hdma_tim1.Init.MemInc              = DMA_MINC_ENABLE;         // increment source address
    hdma_tim1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; // set peripheral data transfer width to 16 bits  
    hdma_tim1.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD; // set memory data transfer width to 16 bits
    hdma_tim1.Init.Mode                = DMA_CIRCULAR;            // configure DMA to circular mode
    hdma_tim1.Init.Priority            = DMA_PRIORITY_LOW;        // set stream priority 
    HAL_DMA_Init(&hdma_tim1);

    __HAL_LINKDMA(&htim1,hdma[TIM_DMA_ID_UPDATE],hdma_tim1);
    
    
    // Initialize Timer
    htim1.Instance               = TIM1;                            // set timer instance  
    htim1.Init.Prescaler         = 8000 - 1;                        // set timer clock prescaler
    htim1.Init.CounterMode       = TIM_COUNTERMODE_UP;              // set counter to UP mode 
    htim1.Init.Period            = 1000 - 1;                        // set timer period
    htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;          // set timer clock division of sampling clock 
    htim1.Init.RepetitionCounter = 0;                               // set repetiotion value to zero 
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;  // set disable shadow preload register 
    HAL_TIM_Base_Init(&htim1);

    
    // Set timer clock source to internal clock
    TIM_ClockConfigTypeDef sClockSourceConfig = {
          .ClockSource = TIM_CLOCKSOURCE_INTERNAL,
    };
    HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);
}

