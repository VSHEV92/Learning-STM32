#include "stm32f4xx_hal.h"
#include "stdio.h"

void Peripheral_Init();

extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart2;

#define EXEMPLE_TIMER htim2
#define EXEMPLE_TIMER_IC_CHANNEL TIM_CHANNEL_2

#define STDIO_UART huart2
