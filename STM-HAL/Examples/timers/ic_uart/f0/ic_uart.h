#include "stm32f0xx_hal.h"
#include "stdio.h"

void Peripheral_Init();
void print_value(uint32_t captured_value);

extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart2;

#define EXEMPLE_TIMER htim2
#define EXEMPLE_TIMER_IC_CHANNEL TIM_CHANNEL_1

#define STDIO_UART huart2
