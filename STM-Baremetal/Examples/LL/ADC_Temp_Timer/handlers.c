#include <stm32f0xx_ll_adc.h>
#include <stm32f0xx_ll_dma.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* str;


void ADC1_COMP_IRQHandler(void) {
    // Clear End-Of-Sequence Flag
    LL_ADC_ClearFlag_EOS(ADC1);

    // Read Raw ADC Data
    uint32_t raw_data = LL_ADC_REG_ReadConversionData32(ADC1);

    // Convert to Celsius
    // VDDA is 3.3V, use 12 bit resolution
    uint32_t temp = __LL_ADC_CALC_TEMPERATURE(3300, raw_data, LL_ADC_RESOLUTION_12B);

    // Allocate memory of 32 bytes in Heap
    str = malloc(32);
    
    // Create string
    sprintf(str, "Temperature: %2d C\n", (int)temp);

    // Configure Address and Data Length
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t)str);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, strlen(str));
    
    // Enable DMA to Start Transfer
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);
  
}


void DMA1_Channel4_5_IRQHandler() {
    // Clean Transfer Completion Interrupt Flag
    LL_DMA_ClearFlag_TC4(DMA1);

    // Disable DMA channel
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);

    // Free Heap Memory
    free(str);

}
