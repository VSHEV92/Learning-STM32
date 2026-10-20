#include <stm32f0xx.h>
#include <main.h>


void SysTick_Handler(void) {
    HAL_IncTick();
}

void USART2_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart2);
}

void DMA1_Channel4_5_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_usart2_tx);
}
