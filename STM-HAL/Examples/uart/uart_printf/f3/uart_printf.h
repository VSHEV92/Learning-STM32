#include "stm32f3xx_hal.h"
#include "stdio.h"

void Peripheral_Init();

extern UART_HandleTypeDef huart1;

#define STDIO_UART huart1
