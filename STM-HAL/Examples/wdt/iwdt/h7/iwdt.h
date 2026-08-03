#include "stm32h7xx_hal.h"
#include "stdio.h"

void Peripheral_Init();

extern IWDG_HandleTypeDef hiwdg;
extern UART_HandleTypeDef huart3;

#define EXAMPLE_IWDG hiwdg
#define STDIO_UART huart3
