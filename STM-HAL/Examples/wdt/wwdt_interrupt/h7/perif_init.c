#include "wwdt_interrupt.h"

// UART3 Handler
UART_HandleTypeDef huart3;
WWDG_HandleTypeDef hwwdg;


void WWDG_IRQHandler(void) {
    HAL_WWDG_IRQHandler(&hwwdg);
}


void Peripheral_Init() {

    // Set APB3 Clock to 4 MHz
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {
        .ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1,
        .APB3CLKDivider = RCC_APB3_DIV16,
    };
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

    // Enable WWD1 system reset 
    HAL_RCCEx_WWDGxSysResetConfig(RCC_WWDG1);

    
    // Enable GPIO Port D and UART 3 clocks
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();
    __HAL_RCC_WWDG1_CLK_ENABLE();


    // Setup GPIO pins
    GPIO_InitTypeDef GPIO_Init = {
        .Pin       = GPIO_PIN_8|GPIO_PIN_9,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_VERY_HIGH,  // set slew rate 
        .Alternate = GPIO_AF7_USART3,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOD, &GPIO_Init);


    // Initalize UART
    huart3.Instance          = USART3;                 // uart interface instance
    huart3.Init.BaudRate     = 9600;                   // buad rate
    huart3.Init.WordLength   = UART_WORDLENGTH_8B;     // bits per symbol
    huart3.Init.StopBits     = UART_STOPBITS_1;        // 1 stop bit
    huart3.Init.Parity       = UART_PARITY_NONE;       // don't use parity bit
    huart3.Init.Mode         = UART_MODE_TX;           // enable uart tx 
    huart3.Init.HwFlowCtl    = UART_HWCONTROL_NONE;    // don't use hardware flow control
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;   // set oversampling to 16 bits  
    HAL_UART_Init(&huart3);


    // Initalize WWDT
    hwwdg.Instance       = WWDG1;             // WWDT Instance
    hwwdg.Init.Prescaler = WWDG_PRESCALER_32; // Clock prescaler
    hwwdg.Init.Window    = 96;                // Widnow value (on half of full period)
    hwwdg.Init.Counter   = 127;               // WWDT reload value
    hwwdg.Init.EWIMode   = WWDG_EWI_ENABLE;   // Enable interrupts
    HAL_WWDG_Init(&hwwdg);


    // Enable WWDT interrupts
    HAL_NVIC_SetPriority(WWDG_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(WWDG_IRQn);

}
