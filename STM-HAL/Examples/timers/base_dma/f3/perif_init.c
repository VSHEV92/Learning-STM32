#include "base_dma.h"

TIM_HandleTypeDef htim1;
DMA_HandleTypeDef hdma_tim1;

// Timer ISR Handler 
void TIM1_UP_TIM16_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim1);
}

// DMA Timer Stream ISR Handler
void DMA1_Channel5_IRQHandler(void)  {
    HAL_DMA_IRQHandler(&hdma_tim1);
}


void Peripheral_Init() {

    // ------------------------------------------------
    // -------- Setup Clock and Interripts ------------
    // ------------------------------------------------

    // Enable Clocks
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_TIM1_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

    // Setup timer interrupts
    HAL_NVIC_SetPriority( TIM1_UP_TIM16_IRQn, 0, 0 );
    HAL_NVIC_EnableIRQ( TIM1_UP_TIM16_IRQn );
    
    // Setup DMA interrupts
    HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

    // ------------------------------------------------
    // --------------- Setup GPIO ---------------------
    // ------------------------------------------------

    // Initialize GPIO pin
    GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin   = GPIO_PIN_15,             // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);


    // ------------------------------------------------
    // ---------------- Setup DMA ---------------------
    // ------------------------------------------------

    // Initialize DMA
    hdma_tim1.Instance                 = DMA1_Channel5;
    hdma_tim1.Init.Direction           = DMA_MEMORY_TO_PERIPH;    // transfer direction Memory to Peripheral
    hdma_tim1.Init.PeriphInc           = DMA_PINC_DISABLE;        // don't increment destination address 
    hdma_tim1.Init.MemInc              = DMA_MINC_ENABLE;         // increment source address
    hdma_tim1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; // set peripheral data transfer width to 16 bits  
    hdma_tim1.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD; // set memory data transfer width to 16 bits
    hdma_tim1.Init.Mode                = DMA_CIRCULAR;            // configure DMA to circular mode
    hdma_tim1.Init.Priority            = DMA_PRIORITY_LOW;        // set stream priority 
    HAL_DMA_Init(&hdma_tim1);

    // Link DMA instanse to timer instanse
    __HAL_LINKDMA(&htim1, hdma[TIM_DMA_ID_UPDATE], hdma_tim1);
    

    // ------------------------------------------------
    // -------------- Setup Timer ---------------------
    // ------------------------------------------------
    
    // Initialize Timer
    htim1.Instance               = TIM1;                            // set timer instance  
    htim1.Init.Prescaler         = 8000 - 1;                        // set timer clock prescaler
    htim1.Init.CounterMode       = TIM_COUNTERMODE_UP;              // set counter to UP mode 
    htim1.Init.Period            = 1000 - 1;                        // set timer period
    htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;          // set timer clock division of sampling clock 
    htim1.Init.RepetitionCounter = 0;                               // set repetiotion value to zero 
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;  // set disable shadow preload register 
    HAL_TIM_Base_Init(&htim1);

}


