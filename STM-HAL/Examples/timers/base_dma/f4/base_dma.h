#include "stm32f4xx_hal.h"

void Peripheral_Init();

extern TIM_HandleTypeDef htim1;

#define EXEMPLE_TIMER htim1

#define LED_PORT GPIOA
#define LED_PIN GPIO_PIN_5
