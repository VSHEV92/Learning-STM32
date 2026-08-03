#include "stm32h7xx_hal.h"
#include "stdio.h"

void Peripheral_Init();

extern WWDG_HandleTypeDef hwwdg;
extern UART_HandleTypeDef huart3;

#define STDIO_UART huart3
#define EXAMPLE_WWDG hwwdg
