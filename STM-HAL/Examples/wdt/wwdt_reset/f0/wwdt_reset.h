#include "stm32f0xx_hal.h"
#include "stdio.h"

void Peripheral_Init();

extern WWDG_HandleTypeDef hwwdg;
extern UART_HandleTypeDef huart2;

#define EXAMPLE_WWDG hwwdg
#define STDIO_UART huart2
