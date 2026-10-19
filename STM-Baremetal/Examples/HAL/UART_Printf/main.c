#include <stm32f0xx.h>
#include <stdio.h>

UART_HandleTypeDef huart2;

// Redirect stdlib stream to UART
int _write(int file, char *ptr, int len)  {
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}


void main() {

    HAL_Init();
    
    int cnt = 0;
    while(1) {
        printf("Counter = %d\n", cnt++);
        HAL_Delay(500);
    }

}
