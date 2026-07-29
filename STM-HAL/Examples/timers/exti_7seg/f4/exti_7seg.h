#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "74HC595.h"

void Peripheral_Init();

extern TIM_HandleTypeDef htim2;

#define EXEMPLE_TIMER htim2

