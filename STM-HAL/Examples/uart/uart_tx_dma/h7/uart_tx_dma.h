#include "stm32h7xx_hal.h"
#include "stdio.h"
#include "string.h"

void Peripheral_Init();

extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart3_tx;

#define STDIO_UART huart3

#define LED_PORT GPIOB
#define LED_PIN GPIO_PIN_0
