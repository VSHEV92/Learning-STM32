#include "base_dma.h"

TIM_HandleTypeDef htim1;
DMA_HandleTypeDef hdma_tim1;

void TIM1_UP_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim1);
}

void DMA1_Stream0_IRQHandler(void)  {
    HAL_DMA_IRQHandler(&hdma_tim1);
}

void Peripheral_Init() {

    // Enable Clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_TIM1_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

    // Initialize GPIO pin
    GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin   = GPIO_PIN_5,             // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    

    // Initialize DMA
    hdma_tim1.Instance                 = DMA1_Stream0;            // set instance to DMA2 Stream 5
    hdma_tim1.Init.Request             = DMA_REQUEST_TIM1_UP;     // DMA request source
    hdma_tim1.Init.Direction           = DMA_MEMORY_TO_PERIPH;    // transfer direction Memory to Peripheral
    hdma_tim1.Init.PeriphInc           = DMA_PINC_DISABLE;        // don't increment destination address 
    hdma_tim1.Init.MemInc              = DMA_MINC_ENABLE;         // increment source address
    hdma_tim1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; // set peripheral data transfer width to 16 bits  
    hdma_tim1.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD; // set memory data transfer width to 16 bits
    hdma_tim1.Init.Mode                = DMA_CIRCULAR;            // configure DMA to circular mode
    hdma_tim1.Init.Priority            = DMA_PRIORITY_LOW;        // set stream priority 
    hdma_tim1.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;    // disable fifo mode
    HAL_DMA_Init(&hdma_tim1);

    // Link DMA instanse to timer instanse
    __HAL_LINKDMA(&htim1, hdma[TIM_DMA_ID_UPDATE], hdma_tim1);


    
    // Set timer clock source to internal clock
    TIM_ClockConfigTypeDef sClockSourceConfig = {
          .ClockSource = TIM_CLOCKSOURCE_INTERNAL,
    };
    HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);
    
    // Initialize Timer
    htim1.Instance               = TIM1;                            // set timer instance  
    htim1.Init.Prescaler         = 64000 - 1;                       // set timer clock prescaler
    htim1.Init.CounterMode       = TIM_COUNTERMODE_UP;              // set counter to UP mode 
    htim1.Init.Period            = 1000 - 1;                        // set timer period
    htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;          // set timer clock division of sampling clock 
    htim1.Init.RepetitionCounter = 0;                               // set repetiotion value to zero 
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;  // set disable shadow preload register 
    HAL_TIM_Base_Init(&htim1);

    // Setup interrupts
    HAL_NVIC_SetPriority( TIM1_UP_IRQn, 0, 0 );
    HAL_NVIC_EnableIRQ( TIM1_UP_IRQn );

    HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
}

