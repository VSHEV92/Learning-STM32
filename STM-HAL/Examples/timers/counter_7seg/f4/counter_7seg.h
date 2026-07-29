#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "74HC595.h"

void Peripheral_Init();

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

#define DISPLAY_UPDATE_TIMER htim2
#define COUNTER_INCREMENT_TIMER htim3

