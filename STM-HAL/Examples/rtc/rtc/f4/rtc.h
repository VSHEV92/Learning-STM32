#include "stm32f4xx_hal.h"
#include "stdio.h"

void Peripheral_Init();

extern RTC_HandleTypeDef hrtc;
extern UART_HandleTypeDef huart2;

#define EXAMPLE_RTC hrtc
#define STDIO_UART huart2
