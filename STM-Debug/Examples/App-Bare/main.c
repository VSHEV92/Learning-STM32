#include <stm32f0xx.h>


void main() {

    HAL_Init();

    while (1) {
	    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(100);
    }

}

