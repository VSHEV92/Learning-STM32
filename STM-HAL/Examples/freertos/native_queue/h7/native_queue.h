#include "stm32h7xx_hal.h"

#include "stdio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void Peripheral_Init();

extern UART_HandleTypeDef huart3;

#define STDIO_UART huart3
