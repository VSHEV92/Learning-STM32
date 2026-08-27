#include "stm32f3xx_hal.h"
#include "cmsis_os.h"
#include "stdio.h"

void Peripheral_Init();

extern UART_HandleTypeDef huart1;

#define STDIO_UART huart1

#define LED_PORT GPIOE
#define LED_PIN GPIO_PIN_15
