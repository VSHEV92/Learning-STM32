#ifndef MAIN_H
#define MAIN_H

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim2;

extern uint16_t adc_data;

#endif

