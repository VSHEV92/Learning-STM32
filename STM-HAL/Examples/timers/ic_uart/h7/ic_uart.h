#include "stm32h7xx_hal.h"
#include "stdio.h"

void Peripheral_Init();

extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart3;

#define EXEMPLE_TIMER htim3
#define EXEMPLE_TIMER_IC_CHANNEL TIM_CHANNEL_4

#define STDIO_UART huart3
