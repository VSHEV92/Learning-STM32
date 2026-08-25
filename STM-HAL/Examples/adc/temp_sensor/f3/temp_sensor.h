#include "stm32f3xx_hal.h"
#include "stdio.h"

void Peripheral_Init();

extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;

#define EXAMPLE_ADC hadc1
#define STDIO_UART huart1

// Temperature Convertion Constants
#define MAX_ADC_VALUE       4095.0
#define REF_TEMP            25.0
#define VOLTAGE_AT_REF_TEMP 1.50
#define ADC_AVG_SLOPE       0.0043
#define BOARD_REF_VLOTAGE   3.3
