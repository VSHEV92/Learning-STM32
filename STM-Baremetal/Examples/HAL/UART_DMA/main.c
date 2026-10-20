#include <stm32f0xx.h>
#include <string.h>
#include <main.h>

char hello_string[] = "Hello from STM32F0\n";

void main() {

    HAL_Init();
    
    while(1) {
        HAL_UART_Transmit_DMA(&huart2, (uint8_t*)hello_string, strlen(hello_string));
        HAL_Delay(500);
    }

}
