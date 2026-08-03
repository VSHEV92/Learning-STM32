#include "stm32h7xx_hal.h"
#include "stdio.h"

void Peripheral_Init();

extern UART_HandleTypeDef huart3;

#define STDIO_UART huart3
