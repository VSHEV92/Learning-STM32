#include <stm32f0xx.h>
#include <string.h>

#define TOGGLE_DELAY_MS 200 

char hello_string[] = "Hello from STM32F0\n";

UART_HandleTypeDef huart2;

void SysTick_Handler(void) {
    static int cnt;
    
    HAL_IncTick();
    
    cnt++;
    if (cnt == TOGGLE_DELAY_MS) {
        HAL_UART_Transmit(&huart2, (uint8_t*)hello_string, strlen(hello_string), HAL_MAX_DELAY);
        cnt = 0;
    }
}

