#include "stm32f3xx_hal.h"
#include "stdio.h"
#include "string.h"

void Peripheral_Init();

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;

#define STDIO_UART huart1

#define LED_PORT GPIOE
#define LED_PIN GPIO_PIN_15
