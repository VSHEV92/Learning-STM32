#include "exti_7seg.h"

TIM_HandleTypeDef htim2;

// Data for ISR porcessing
uint32_t segment = 0;
uint32_t counter = 0;

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

// Update 7-segments LED on timer overflow
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    shiftRegisters[0] = (1 << segment);
    shiftRegisters[1] = data_7seg[segment];
    ShiftRegister74HC595_update();

    segment = (segment + 1) % 4;
}

// Increment counter on button press
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    char counter_string[5];

    counter = (counter + 1) % 10000;

    snprintf(counter_string, sizeof(counter_string), "%04u", (unsigned int)counter);

    for (int i = 0; i < 4; i++) {
        data_7seg[i] = digit_to_7seg(counter_string[i]);
    }
}

void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim2);
}

void EXTI1_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
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

    // Enable Clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    __HAL_RCC_TIM2_CLK_ENABLE();

    // Initialize GPIO pin
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
    
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // Initialize Timer
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 1600 - 1;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 10;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&htim2);

    // Setup interrupts
    HAL_NVIC_SetPriority( TIM2_IRQn, 0, 0 );
    HAL_NVIC_EnableIRQ(TIM2_IRQn );

    HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    // Start timers in interrupt mode 
    HAL_TIM_Base_Start_IT(&htim2);

    while (1) {}
}

