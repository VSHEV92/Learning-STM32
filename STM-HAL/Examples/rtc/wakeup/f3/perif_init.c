#include "wakeup.h"


// Peripheral Handlers
UART_HandleTypeDef huart1;
RTC_HandleTypeDef hrtc;

void RTC_WKUP_IRQHandler(void) {
    HAL_RTCEx_WakeUpTimerIRQHandler(&hrtc);
}

void Peripheral_Init() {
    
    // Enable GPIO Port C and UART 1 clocks
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();


    // Enable LSI to source RTC
    RCC_OscInitTypeDef RCC_OscInitStruct = {
        .OscillatorType = RCC_OSCILLATORTYPE_LSI,
        .LSIState = RCC_LSI_ON,
    };
    HAL_RCC_OscConfig(&RCC_OscInitStruct);


    // Select LSI as RTC source
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {
        .PeriphClockSelection = RCC_PERIPHCLK_RTC,
        .RTCClockSelection = RCC_RTCCLKSOURCE_LSI,
    };
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);


    // Setup GPIO pins
    GPIO_InitTypeDef GPIO_Init = {
        .Pin       = GPIO_PIN_4|GPIO_PIN_5,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_HIGH,       // set slew rate 
        .Alternate = GPIO_AF7_USART1,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOC, &GPIO_Init);


    // Initalize UART
    huart1.Instance          = USART1;                 // uart interface instance
    huart1.Init.BaudRate     = 9600;                   // buad rate
    huart1.Init.WordLength   = UART_WORDLENGTH_8B;     // bits per symbol
    huart1.Init.StopBits     = UART_STOPBITS_1;        // 1 stop bit
    huart1.Init.Parity       = UART_PARITY_NONE;       // don't use parity bit
    huart1.Init.Mode         = UART_MODE_TX;           // enable uart tx 
    huart1.Init.HwFlowCtl    = UART_HWCONTROL_NONE;    // don't use hardware flow control
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;   // set oversampling to 16 bits  
    HAL_UART_Init(&huart1);


    // Initialize RTC
    hrtc.Instance            = RTC;                       // RTC instance    
    hrtc.Init.AsynchPrediv   = 127;                       // first clock prescaler 
    hrtc.Init.SynchPrediv    = 255;                       // second clock prescaler
    HAL_RTC_Init(&hrtc);


    // Start Wake Up Timer
    HAL_RTCEx_SetWakeUpTimer_IT(
        &hrtc,                          // RTC instance
        2048,                           // Wake Up Period
        RTC_WAKEUPCLOCK_RTCCLK_DIV16    // Wake Up Clock - RTC Clock divided by 16
    );


    // Enable Wake Up Interrupts
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}
