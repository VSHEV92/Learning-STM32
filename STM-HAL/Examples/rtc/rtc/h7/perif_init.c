#include "rtc.h"

// UART3 Handler
UART_HandleTypeDef huart3;
RTC_HandleTypeDef hrtc;

void RTC_Alarm_IRQHandler(void) {
    HAL_RTC_AlarmIRQHandler(&hrtc);
}

void Peripheral_Init() {

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
    
    // Enable clocks
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();
    __HAL_RCC_RTC_ENABLE();


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

    // Initialize RTC
    hrtc.Instance            = RTC;                       // RTC instance    
    hrtc.Init.HourFormat     = RTC_HOURFORMAT_24;         // set format to 24 hours (no am/pm)
    hrtc.Init.AsynchPrediv   = 127;                       // first clock prescaler 
    hrtc.Init.SynchPrediv    = 255;                       // second clock prescaler
    HAL_RTC_Init(&hrtc);


    // setup RTC time
    RTC_TimeTypeDef sTime = {
        .Hours = 0x0,
        .Minutes = 0x0,
        .Seconds = 0x0,
    };
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);

    // RTC Alarm Settings
    RTC_AlarmTypeDef sAlarm = {
        .AlarmMask = RTC_ALARMMASK_ALL,  // don't mask any time fields
        .Alarm = RTC_ALARM_A,            // use ALARM A
    };
    HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD);

    // Enable RTC Interrupts
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);

}
