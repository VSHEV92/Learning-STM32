#include <stm32f0xx.h>

#define TOGGLE_DELAY_MS 200

void main() {

    HAL_Init();

    __HAL_RCC_GPIOB_CLK_ENABLE();


    GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin   = GPIO_PIN_3,             // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


    while(1) {
	    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
	    HAL_Delay(TOGGLE_DELAY_MS);
    }
    
}
