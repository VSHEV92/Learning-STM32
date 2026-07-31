#include "stm32f4xx_hal.h"

#include "FreeRTOS.h"
#include "task.h"

#define LED_PORT GPIOA
#define LED_PIN GPIO_PIN_5

void Peripheral_Init();
