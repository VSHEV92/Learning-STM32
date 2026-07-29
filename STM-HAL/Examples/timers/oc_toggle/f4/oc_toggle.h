#include "stm32f4xx_hal.h"

void Peripheral_Init();

extern TIM_HandleTypeDef htim2;

#define EXEMPLE_TIMER htim2
#define EXEMPLE_TIMER_OC_CHANNEL TIM_CHANNEL_1

