#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "74HC595.h"

void Peripheral_Init();

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

#define EXAMPLE_ADC hadc1
#define DISPLAY_UPDATE_TIMER htim2
#define ADC_VALUE_UPDATE_TIMER htim3
