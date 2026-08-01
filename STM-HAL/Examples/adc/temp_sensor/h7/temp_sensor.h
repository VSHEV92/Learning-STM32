#include "stm32h7xx_hal.h"
#include "stdio.h"

void Peripheral_Init();

extern ADC_HandleTypeDef hadc3;
extern UART_HandleTypeDef huart3;

#define EXAMPLE_ADC hadc3
#define STDIO_UART huart3

// Temperature Convertion Constants
#define MAX_ADC_VALUE       65535.0
#define REF_TEMP            30.0
#define VOLTAGE_AT_REF_TEMP 0.62
#define ADC_AVG_SLOPE       0.0025
#define BOARD_REF_VLOTAGE   3.3
