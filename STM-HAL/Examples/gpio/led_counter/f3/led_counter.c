#include "led_counter.h"

void Led_Counter_Increment() {
    static uint32_t counter = 0;

    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, ((counter >> 0) & 0x1) );
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, ((counter >> 1) & 0x1) );
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, ((counter >> 2) & 0x1) );
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, ((counter >> 3) & 0x1) );
    
    counter++;
}
