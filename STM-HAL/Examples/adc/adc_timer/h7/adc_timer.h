#include "stm32h7xx_hal.h"
#include "stdio.h"

void Peripheral_Init();

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart3;

#define EXAMPLE_ADC hadc1
#define EXAMPLE_TIMER htim2
#define STDIO_UART huart3

