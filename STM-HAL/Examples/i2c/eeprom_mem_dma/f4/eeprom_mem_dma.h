#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "string.h"

void Peripheral_Init();

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

#define EXAMPLE_I2C hi2c1
#define STDIO_UART huart2

