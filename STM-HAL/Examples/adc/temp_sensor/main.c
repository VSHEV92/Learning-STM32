#include "temp_sensor.h"


// Set UART 2 interface as input/output stream
int __io_putchar(int ch) {
  HAL_UART_Transmit(&STDIO_UART, (uint8_t *)&ch, 1, HAL_MAX_DELAY); 
  return ch;
}

void main() {

    /*
     * 1. Enable instruction and data cache. Enable instruction prefetch.
     * 2. Set NVIC priority grouping to 4 bits 
     * 3. Call HAL_InitTick(), which:
     *    - set Systick to 1ms period
     *    - set Systick IRQ priority to TICK_INT_PRIORITY (minimum).
     */
    HAL_Init();


    /*
     *  Initialize example peripheral
     */
    Peripheral_Init();
    

    while (1) {
        HAL_ADC_Start(&EXAMPLE_ADC);
    	HAL_ADC_PollForConversion(&EXAMPLE_ADC, HAL_MAX_DELAY);
    
    	uint16_t adc_data = HAL_ADC_GetValue(&EXAMPLE_ADC);
    
    	float adc_voltage = ((float)adc_data * BOARD_REF_VLOTAGE) / MAX_ADC_VALUE;
    
    	float temperature = ((adc_voltage - VOLTAGE_AT_REF_TEMP) / ADC_AVG_SLOPE) + REF_TEMP;

        int integer = temperature;
        int fraction = (temperature - integer) * 100;
        printf("Temperature: %d.%02d C\n", integer, fraction);
    
    	HAL_ADC_Stop(&EXAMPLE_ADC);
    	HAL_Delay(500);
    }
}

