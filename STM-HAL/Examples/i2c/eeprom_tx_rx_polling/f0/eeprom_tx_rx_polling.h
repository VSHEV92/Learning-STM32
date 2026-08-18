#include "stm32f0xx_hal.h"
#include "stdio.h"

void print_int(int value, int number_of_digits);

void Peripheral_Init();

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

#define EXAMPLE_I2C hi2c1
#define STDIO_UART huart2
