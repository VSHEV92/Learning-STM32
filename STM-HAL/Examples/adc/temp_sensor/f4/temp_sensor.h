#include "stm32f4xx_hal.h"
#include "stdio.h"

void Peripheral_Init();

extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;

#define EXAMPLE_ADC hadc1
#define STDIO_UART huart2
