#include <stm32f0xx.h>
#include <string.h>

char hello_string[] = "Hello from STM32F0\n";

UART_HandleTypeDef huart2;

void main() {

    HAL_Init();
    
    while(1) {
        HAL_UART_Transmit(&huart2, (uint8_t*)hello_string, strlen(hello_string), HAL_MAX_DELAY);
        HAL_Delay(500);
    }

}
