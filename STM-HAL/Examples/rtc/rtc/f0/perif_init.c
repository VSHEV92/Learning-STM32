#include "rtc.h"
#include "math.h"

// Peripheral Handlers
UART_HandleTypeDef huart2;
RTC_HandleTypeDef hrtc;

void print_int(int value, int number_of_digits) {

    int base = pow(10, number_of_digits);
    uint8_t digit;

    for (int i = 0; i < number_of_digits; i++) {
        value %= base;
        base /= 10;

        digit = value / base + '0';

        HAL_UART_Transmit(&huart2, &digit, 1, HAL_MAX_DELAY);
    }

}

void RTC_IRQHandler(void) {
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


    // Enable GPIO Port A and UART 2 clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_RTC_ENABLE();


    // Setup GPIO pins
    GPIO_InitTypeDef GPIO_Init = {
        .Pin       = GPIO_PIN_2|GPIO_PIN_15,     // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_HIGH,       // set slew rate 
        .Alternate = GPIO_AF1_USART2,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOA, &GPIO_Init);


    // Initalize UART
    huart2.Instance          = USART2;                 // uart interface instance
    huart2.Init.BaudRate     = 9600;                   // buad rate
    huart2.Init.WordLength   = UART_WORDLENGTH_8B;     // bits per symbol
    huart2.Init.StopBits     = UART_STOPBITS_1;        // 1 stop bit
    huart2.Init.Parity       = UART_PARITY_NONE;       // don't use parity bit
    huart2.Init.Mode         = UART_MODE_TX;           // enable uart tx 
    huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;    // don't use hardware flow control
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;   // set oversampling to 16 bits  
    HAL_UART_Init(&huart2);


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
    HAL_NVIC_SetPriority(RTC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(RTC_IRQn);
}


