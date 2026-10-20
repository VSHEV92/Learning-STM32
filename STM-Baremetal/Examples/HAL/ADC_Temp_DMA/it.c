#include <stm32f0xx.h>
#include <stm32f0xx_ll_adc.h>

#include <string.h>
#include <stdio.h>

#include <main.h>

uint16_t adc_data;

char uart_str[30];

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    // Convert ADC value to Celsius
    int temp = __LL_ADC_CALC_TEMPERATURE(VDD_VALUE, adc_data, ADC_RESOLUTION_12B);

    // Create string for UART
    sprintf(uart_str, "Temperature: %d C\n", temp);

    // Start UART transfer using DMA
    HAL_UART_Transmit_DMA(&huart2, (uint8_t*)uart_str, strlen(uart_str));
}


void SysTick_Handler(void) {
    HAL_IncTick();
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

void DMA1_Channel1_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_adc1);
}

