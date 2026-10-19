#include <stm32f0xx.h>
#include <stm32f0xx_ll_adc.h>
#include <stdio.h>

ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;

// Redirect stdlib stream to UART
int _write(int file, char *ptr, int len)  {
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}


void main() {

    HAL_Init();
    
    while(1) {
        HAL_ADC_Start(&hadc1);
    	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    
    	uint16_t adc_data = HAL_ADC_GetValue(&hadc1);

        int temp = __LL_ADC_CALC_TEMPERATURE(VDD_VALUE, adc_data, ADC_RESOLUTION_12B);

        printf("Temperature: %d C\n", temp);
        HAL_Delay(500);
    }

}
