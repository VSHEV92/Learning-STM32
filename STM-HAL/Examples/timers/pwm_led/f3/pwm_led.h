#include "stm32f3xx_hal.h"

void Peripheral_Init();

extern TIM_HandleTypeDef htim1;

#define EXEMPLE_TIMER htim1
#define EXEMPLE_TIMER_PWM_CHANNEL TIM_CHANNEL_4
