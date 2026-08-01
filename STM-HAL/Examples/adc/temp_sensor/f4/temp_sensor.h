#include "stm32f4xx_hal.h"
#include "stdio.h"

void Peripheral_Init();

extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;

#define EXAMPLE_ADC hadc1
#define STDIO_UART huart2

// Temperature Convertion Constants
#define MAX_ADC_VALUE       4095.0
#define REF_TEMP            25.0
#define VOLTAGE_AT_REF_TEMP 0.76
#define ADC_AVG_SLOPE       0.0025
#define BOARD_REF_VLOTAGE   3.3
