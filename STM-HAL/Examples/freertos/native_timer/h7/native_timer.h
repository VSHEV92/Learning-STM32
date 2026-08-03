#include "stm32h7xx_hal.h"

#include "stdio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"


void Peripheral_Init();

extern UART_HandleTypeDef huart3;

#define STDIO_UART huart3

#define LED_PORT GPIOB
#define LED_PIN GPIO_PIN_0
