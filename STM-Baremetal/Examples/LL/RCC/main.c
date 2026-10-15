#include <stm32f0xx_ll_cortex.h>
#include <stm32f0xx_ll_rcc.h>
#include <stm32f0xx_ll_bus.h>
#include <stm32f0xx_ll_gpio.h>
#include <stm32f0xx_ll_usart.h>
#include <stm32f0xx_ll_utils.h>

#include <stdio.h>

#define UART_BAUDRATE 9600


int _write(int file, char *ptr, int len)  {
    for (int i = 0; i < len; i++) {
        while ( ! LL_USART_IsActiveFlag_TXE(USART2)) {}
        LL_USART_TransmitData8(USART2, ptr[i]);
    }
    return len;
}

void RCC_Init() {
    // Configure PLL Clock to 32 MHz

    // 1. Configure the PLL parameters
    //    Set PLL PREDIV value to 4
    //    Set PLL Multiplication to 16
    LL_UTILS_PLLInitTypeDef pll_init;
    pll_init.PLLMul = LL_RCC_PLL_MUL_16;     
    pll_init.PLLDiv = LL_RCC_PREDIV_DIV_4;


    // 2. Set APB clock Divider to 4
    LL_UTILS_ClkInitTypeDef clk_init;
    clk_init.AHBCLKDivider  = LL_RCC_SYSCLK_DIV_1;
    clk_init.APB1CLKDivider = LL_RCC_APB1_DIV_4;

    // 3. Choose HSI/PREDIV as PLL Source
    //    PLL input frequency is 2 MHz
    //    PLL Output will be 32 MHz
    //    So UART2 Clock will be 8 MHz
    LL_PLL_ConfigSystemClock_HSI(&pll_init, &clk_init);

}


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
    LL_USART_Enable(USART2);
}

void SysTick_Init() {
    LL_LPM_EnableSleepOnExit();
    SysTick_Config( SystemCoreClock / 2 );
}


void SysTick_Handler() {
    LL_RCC_ClocksTypeDef clocks;
    
    LL_RCC_GetSystemClocksFreq(&clocks);

    printf("STM32F0 AHB clock: %ld Hz\n", clocks.HCLK_Frequency);
    printf("STM32F0 APB clock: %ld Hz\n", clocks.PCLK1_Frequency);
    printf("\n");
}

    

void main() {
    RCC_Init();
    GPIO_Init();
    UART_Init();
    SysTick_Init();

    __WFI();
}

