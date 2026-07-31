#include "stm32f4xx_hal.h"

#include "stdio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"


void Peripheral_Init();

extern UART_HandleTypeDef huart2;

#define STDIO_UART huart2

#define LED_PORT GPIOA
#define LED_PIN GPIO_PIN_5
