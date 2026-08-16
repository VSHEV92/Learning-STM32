#include "stm32f0xx_hal.h"

void Peripheral_Init();

extern TIM_HandleTypeDef htim1;

#define EXEMPLE_TIMER htim1

#define LED_PORT GPIOB
#define LED_PIN GPIO_PIN_3
