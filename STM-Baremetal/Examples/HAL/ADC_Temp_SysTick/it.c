#include <stm32f0xx.h>
#include <stm32f0xx_ll_adc.h>

#include <string.h>
#include <stdio.h>

#include <main.h>

#define TOGGLE_DELAY_MS 200 


char uart_str[30];

void SysTick_Handler(void) {
    static int cnt;
    
    HAL_IncTick();
    
    cnt++;
    if (cnt == TOGGLE_DELAY_MS) {

        // Start Conversion on SysTick
        HAL_ADC_Start_IT(&hadc1);
        
        cnt = 0;
    }
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    // Get ADC value and convert it to Celsius
    uint16_t adc_data = HAL_ADC_GetValue(&hadc1);
    int temp = __LL_ADC_CALC_TEMPERATURE(VDD_VALUE, adc_data, ADC_RESOLUTION_12B);

    // Create string for UART
    sprintf(uart_str, "Temperature: %d C\n", temp);

    // Start UART transfer using DMA
    HAL_UART_Transmit_DMA(&huart2, (uint8_t*)uart_str, strlen(uart_str));
}


void ADC1_IRQHandler(void) {
    HAL_ADC_IRQHandler(&hadc1);
}

void USART2_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart2);
}

void DMA1_Channel4_5_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_usart2_tx);
}


