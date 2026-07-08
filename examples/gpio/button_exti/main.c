#include "button_exti.h"

// external interrupt ISR
void EXTI1_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

// external interrupt Callback
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_1) {
	    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    }
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

    // Enable GPIO Clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Initialize GPIO Led Pin
    GPIO_InitTypeDef GPIO_InitStruct_Led = {
        .Pin   = GPIO_PIN_5,             // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct_Led);
    
    // Initialize GPIO Button Pin
    GPIO_InitTypeDef GPIO_InitStruct_Button = {
        .Pin   = GPIO_PIN_1,             // pin number
        .Mode  = GPIO_MODE_IT_RISING,    // external interrupt on rising edge mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
    };
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct_Button);

    // Configure interrupts to maximum priority (lowest value)
    HAL_NVIC_SetPriority(
        EXTI1_IRQn, // IRQ number
        0,          // Preempt Priority
        0           // Sub Priority
    );
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    while (1) {}
}

