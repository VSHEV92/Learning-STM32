#include "led_counter.h"

void Led_Counter_Increment() {
    static uint32_t counter = 0;

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5,  !((counter >> 0) & 0x1) );
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,  !((counter >> 1) & 0x1) );
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,  !((counter >> 2) & 0x1) );
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, !((counter >> 3) & 0x1) );
    
    counter++;
}
