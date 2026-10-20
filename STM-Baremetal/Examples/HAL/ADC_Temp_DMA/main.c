#include <stm32f0xx.h>
#include <main.h>

void main() {
    HAL_Init();

    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc_data, 1);
    HAL_TIM_Base_Start(&htim2);
    
    __WFI();
}
