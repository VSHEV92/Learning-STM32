#include "stm32f0xx_hal.h"

void Peripheral_Init();

extern TIM_HandleTypeDef htim2;

#define EXEMPLE_TIMER htim2
#define EXEMPLE_TIMER_PWM_CHANNEL TIM_CHANNEL_2
