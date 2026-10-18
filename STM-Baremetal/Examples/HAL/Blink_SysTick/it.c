#include <stm32f0xx.h>

#define TOGGLE_DELAY_MS 200 

void SysTick_Handler(void) {
    static int cnt;
    
    HAL_IncTick();
    
    cnt++;
    if (cnt == TOGGLE_DELAY_MS) {
	    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
        cnt = 0;
    }
}

