#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "string.h"

void Peripheral_Init();

extern UART_HandleTypeDef huart2;

#define STDIO_UART huart2
