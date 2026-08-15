#include "stm32f0xx_hal.h"
#include "stdio.h"

void Peripheral_Init();

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_tx;

#define STDIO_UART huart2

#define LED_PORT GPIOB
#define LED_PIN GPIO_PIN_3

