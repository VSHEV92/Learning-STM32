#include "stm32f0xx_hal.h"
#include "stdio.h"

void Peripheral_Init();

void print_int(int value, int number_of_digits);

extern RTC_HandleTypeDef hrtc;
extern UART_HandleTypeDef huart2;

#define EXAMPLE_RTC hrtc
#define STDIO_UART huart2
