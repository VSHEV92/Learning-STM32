#include "stm32f3xx_hal.h"

#include "FreeRTOS.h"
#include "task.h"

#define LED_PORT GPIOE
#define LED_PIN GPIO_PIN_15

void Peripheral_Init();
