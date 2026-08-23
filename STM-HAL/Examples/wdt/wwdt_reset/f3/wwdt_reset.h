#include "stm32f3xx_hal.h"
#include "stdio.h"

void Peripheral_Init();

extern WWDG_HandleTypeDef hwwdg;
extern UART_HandleTypeDef huart1;

#define EXAMPLE_WWDG hwwdg
#define STDIO_UART huart1
