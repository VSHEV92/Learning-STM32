#include <stm32f0xx_ll_cortex.h>
#include <stm32f0xx_ll_bus.h>
#include <stm32f0xx_ll_gpio.h>
#include <stm32f0xx_ll_usart.h>
#include <stm32f0xx_ll_adc.h>
#include <stm32f0xx_ll_dma.h>
#include <stm32f0xx_ll_tim.h>

extern uint16_t adc_data;

void GPIO_Init() {
    
    // Enable Clock
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

    // Configure GPIO function for UART
    LL_GPIO_InitTypeDef gpio_init;
    LL_GPIO_StructInit(&gpio_init);

    gpio_init.Pin = LL_GPIO_PIN_2 | LL_GPIO_PIN_15;
    gpio_init.Mode = LL_GPIO_MODE_ALTERNATE;          
    gpio_init.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpio_init.Pull = LL_GPIO_PULL_NO;              
    gpio_init.Alternate = LL_GPIO_AF_1;              
    LL_GPIO_Init (GPIOA, &gpio_init);
}


void UART_Init() {

    // Enable Clock
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

    // Configure UART to 9600 baudrate, 8 data, 1 stop, 0 parity
    LL_USART_InitTypeDef uart_init;
    LL_USART_StructInit(&uart_init);

    uart_init.BaudRate = 9600;
    uart_init.DataWidth = LL_USART_DATAWIDTH_8B;
    uart_init.StopBits = LL_USART_STOPBITS_1;
    uart_init.Parity = LL_USART_PARITY_NONE;
    uart_init.TransferDirection = LL_USART_DIRECTION_TX;
    uart_init.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    uart_init.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init (USART2, &uart_init);
    
    // Enable Request data from DMA on TXE flag
    LL_USART_EnableDMAReq_TX(USART2);

    // Enable UART
    LL_USART_Enable(USART2);
}


void ADC_Init() {

    // Enable Clock
    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_ADC1);

    // Calibrate ADC
    LL_ADC_StartCalibration(ADC1);
    while ( LL_ADC_IsCalibrationOnGoing(ADC1) ) {};

    // Enable Temperature Sensor
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_TEMPSENSOR);

    // Configure clock source, resolution and alignment
    LL_ADC_InitTypeDef adc_init;
    LL_ADC_StructInit(&adc_init);

    adc_init.Clock = LL_ADC_CLOCK_SYNC_PCLK_DIV4;
    adc_init.Resolution = LL_ADC_RESOLUTION_12B;
    adc_init.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
    LL_ADC_Init (ADC1, &adc_init);


    // Configure conversion as single, trigger as software
    LL_ADC_REG_InitTypeDef adc_reg_init;
    LL_ADC_REG_StructInit(&adc_reg_init);
    
    adc_reg_init.TriggerSource    = LL_ADC_REG_TRIG_EXT_TIM2_TRGO;
    adc_reg_init.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
    adc_reg_init.ContinuousMode   = LL_ADC_REG_CONV_SINGLE;
    adc_reg_init.DMATransfer      = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
    LL_ADC_REG_Init(ADC1, &adc_reg_init);

    // Enable Temperature Channel for Conversion
    LL_ADC_REG_SetSequencerChannels(ADC1, LL_ADC_CHANNEL_TEMPSENSOR);

    // Set Sample time to maximum value
    LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_239CYCLES_5); 

    // Enable ADC
    LL_ADC_Enable(ADC1);
    while ( !LL_ADC_IsActiveFlag_ADRDY(ADC1) );

    // Start Trigger Waiting
    LL_ADC_REG_StartConversion(ADC1);
}


void DMA_Init() {
    // Enable DMA
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

    LL_DMA_InitTypeDef dma_init;

    // Configure Channel 4 for UART TX
    LL_DMA_StructInit(&dma_init);
    
    dma_init.PeriphOrM2MSrcAddress = (uint32_t)&(USART2->TDR);
    dma_init.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
    dma_init.Mode = LL_DMA_MODE_NORMAL;
    dma_init.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
    dma_init.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
    dma_init.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
    dma_init.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;

    LL_DMA_Init(DMA1, LL_DMA_CHANNEL_4, &dma_init);

    // Configure Channel 1 for ADC 
    LL_DMA_StructInit(&dma_init);
    
    dma_init.PeriphOrM2MSrcAddress = (uint32_t)&(ADC1->DR);
    dma_init.MemoryOrM2MDstAddress = (uint32_t)&adc_data;
    dma_init.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
    dma_init.Mode = LL_DMA_MODE_CIRCULAR;
    dma_init.NbData = 1;
    dma_init.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
    dma_init.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_NOINCREMENT;
    dma_init.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
    dma_init.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;

    LL_DMA_Init(DMA1, LL_DMA_CHANNEL_1, &dma_init);

    // Enable DMA Interrupts
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_4);
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);

    NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);

    // Enable DMA 1 Channel to Wait for ADC data
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
}


void TIM2_Init() {
    // Enable Clock
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

    // Set TIM2 period 500 ms
    // APB is 8 MHz, divider is 1
    LL_TIM_InitTypeDef tim_init;
    LL_TIM_StructInit( &tim_init);   

    tim_init.Prescaler = 8000 - 1;
    tim_init.CounterMode = LL_TIM_COUNTERMODE_UP;
    tim_init.Autoreload = 500 - 1;
    LL_TIM_Init(TIM2, &tim_init);

    // Set Output trigger on update event
    LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_UPDATE);

    // Enable Timer
    LL_TIM_EnableCounter(TIM2);
}


void Power_Init() {
    // Enable Sleep-On-Exit 
    LL_LPM_EnableSleepOnExit();
}


void MCU_Init() {
    GPIO_Init();
    UART_Init();
    ADC_Init();
    DMA_Init();
    Power_Init();
    TIM2_Init();
}

