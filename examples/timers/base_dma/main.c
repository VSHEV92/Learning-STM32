#include "base_dma.h"

TIM_HandleTypeDef htim1;
DMA_HandleTypeDef hdma_tim1;

// ------------------------------------------------
// ------------- Setup DMA ISR --------------------
// ------------------------------------------------

// Timer ISR Handler 
void TIM1_UP_TIM10_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim1);
}

// DMA Timer Stream ISR Handler
void DMA2_Stream5_IRQHandler(void)  {
    HAL_DMA_IRQHandler(&hdma_tim1);
}

// Send half data to timer period register
void HAL_TIM_PeriodElapsedHalfCpltCallback(TIM_HandleTypeDef *htim) {
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

// Send all data to timer period register
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
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

    // ------------------------------------------------
    // -------- Setup Clock and Interripts ------------
    // ------------------------------------------------

    // Enable Clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_TIM1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();

    // Setup timer interrupts
    HAL_NVIC_SetPriority( TIM1_UP_TIM10_IRQn, 0, 0 );
    HAL_NVIC_EnableIRQ( TIM1_UP_TIM10_IRQn );
    
    // Setup DMA interrupts
    HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);

    // ------------------------------------------------
    // --------------- Setup GPIO ---------------------
    // ------------------------------------------------

    // Initialize GPIO pin
    GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin   = GPIO_PIN_5,             // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


    // ------------------------------------------------
    // ---------------- Setup DMA ---------------------
    // ------------------------------------------------

    // Initialize DMA
    hdma_tim1.Instance                 = DMA2_Stream5;            // set instance to DMA2 Stream 5
    hdma_tim1.Init.Channel             = DMA_CHANNEL_6;           // set channel
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
    

    // ------------------------------------------------
    // -------------- Setup Timer ---------------------
    // ------------------------------------------------

    // Set timer clock source to internal clock
    TIM_ClockConfigTypeDef sClockSourceConfig = {
          .ClockSource = TIM_CLOCKSOURCE_INTERNAL,
    };
    HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);
    
    // Initialize Timer
    htim1.Instance               = TIM1;                            // set timer instance  
    htim1.Init.Prescaler         = 16000 - 1;                       // set timer clock prescaler
    htim1.Init.CounterMode       = TIM_COUNTERMODE_UP;              // set counter to UP mode 
    htim1.Init.Period            = 1000 - 1;                        // set timer period
    htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;          // set timer clock division of sampling clock 
    htim1.Init.RepetitionCounter = 0;                               // set repetiotion value to zero 
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;  // set disable shadow preload register 
    HAL_TIM_Base_Init(&htim1);

    // Timer update periods
    uint16_t periods[2] = {
        200  - 1,
	    1000  - 1
    };

    // Start timer in DMA mode 
    HAL_TIM_Base_Start_DMA(&htim1, (const uint32_t*)periods, 2);

    while (1) {}
}

