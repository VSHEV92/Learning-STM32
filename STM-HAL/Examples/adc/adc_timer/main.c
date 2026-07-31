#include "adc_timer.h"


// Set UART interface as input/output stream
int __io_putchar(int ch) {
  HAL_UART_Transmit(&STDIO_UART, (uint8_t *)&ch, 1, HAL_MAX_DELAY); 
  return ch;
}

// ------------------------------------------------
// ------------- Setup ADC ISR --------------------
// ------------------------------------------------

// ADC IRQ Handler
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    uint16_t adc_data = HAL_ADC_GetValue(hadc);
    printf("ADC Data: %u\n", (unsigned int)adc_data);
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


    HAL_ADC_Start_IT(&EXAMPLE_ADC);
    HAL_TIM_Base_Start(&EXAMPLE_TIMER);

    while (1) {}
}

