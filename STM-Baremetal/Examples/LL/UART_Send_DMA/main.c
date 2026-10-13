
#include <stm32f0xx_ll_cortex.h>
#include <stm32f0xx_ll_bus.h>
#include <stm32f0xx_ll_gpio.h>
#include <stm32f0xx_ll_usart.h>
#include <stm32f0xx_ll_dma.h>

#include <string.h>

#define UART_BAUDRATE 9600

char UART_String[] = "Hello STM32F0\n";



void GPIO_Init() {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

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
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

    LL_USART_InitTypeDef uart_init;
    LL_USART_StructInit(&uart_init);

    uart_init.BaudRate = UART_BAUDRATE;
    uart_init.DataWidth = LL_USART_DATAWIDTH_8B;
    uart_init.StopBits = LL_USART_STOPBITS_1;
    uart_init.Parity = LL_USART_PARITY_NONE;
    uart_init.TransferDirection = LL_USART_DIRECTION_TX;
    uart_init.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    uart_init.OverSampling = LL_USART_OVERSAMPLING_16;

    LL_USART_Init (USART2, &uart_init);
    
    LL_USART_EnableDMAReq_TX(USART2);
    LL_USART_Enable(USART2);
}


void DMA_Init() {

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);

    LL_DMA_InitTypeDef dma_init;
    LL_DMA_StructInit(&dma_init);
    
    dma_init.PeriphOrM2MSrcAddress = (uint32_t)&(USART2->TDR);
    dma_init.MemoryOrM2MDstAddress = (uint32_t)UART_String;
    dma_init.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
    dma_init.Mode = LL_DMA_MODE_NORMAL;
    dma_init.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
    dma_init.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
    dma_init.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
    dma_init.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;

    LL_DMA_Init(DMA1, LL_DMA_CHANNEL_4, &dma_init);
}


void SysTick_Init() {
    // Set Sleep-On-Exit Bit
    LL_LPM_EnableSleepOnExit();

    // Configure SysTick to fire every 250 ms
    SysTick_Config( SystemCoreClock/4 );
}


// SysTick Handler
void SysTick_Handler() {
    // Disable DMA
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);

    // Set Transer size to string Length
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, strlen(UART_String));
    
    // Start Transfer
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);
}


void main() {
    GPIO_Init();
    UART_Init();
    DMA_Init();
    SysTick_Init();

    __WFI();
}

