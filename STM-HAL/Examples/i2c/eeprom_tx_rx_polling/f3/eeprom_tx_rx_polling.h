#include "stm32f3xx_hal.h"
#include "stdio.h"

void Peripheral_Init();

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;

#define EXAMPLE_I2C hi2c1
#define STDIO_UART huart1
