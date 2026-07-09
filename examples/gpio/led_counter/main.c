#include "led_counter.h"

#define INCREMENT_DELAY 200

void main() {

    /*
     * 1. Enable instruction and data cache. Enable instruction prefetch.
     * 2. Set NVIC priority grouping to 4 bits 
     * 3. Call HAL_InitTick(), which:
     *    - set Systick to 1ms period
     *    - set Systick IRQ priority to TICK_INT_PRIORITY (minimum).
     */
    HAL_Init();

    // Enable GPIO clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Initialize GPIO pins
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin   = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin   = GPIO_PIN_6;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    uint32_t counter = 0;
    while (1) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, !((counter >> 0) & 0x1) );
	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, !((counter >> 1) & 0x1) );
	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, !((counter >> 2) & 0x1) );
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, !((counter >> 3) & 0x1) );

	    HAL_Delay(INCREMENT_DELAY);
	    counter++;
    }
}

