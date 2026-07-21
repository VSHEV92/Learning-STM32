#include "rtc.h"

// Peripheral Handlers
UART_HandleTypeDef huart2;
RTC_HandleTypeDef hrtc;

// Set UART 2 interface as input/output stream
int __io_putchar(int ch) {
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY); 
  return ch;
}

// RTC ISR Handler
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) {
    printf("Alarm callback is triggered\n");
}

void RTC_Alarm_IRQHandler(void) {
    HAL_RTC_AlarmIRQHandler(&hrtc);
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

    
    // Enable GPIO Port A and UART 2 clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();


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
        .Pin       = GPIO_PIN_2|GPIO_PIN_3,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_VERY_HIGH,  // set slew rate 
        .Alternate = GPIO_AF7_USART2,            // choose UART2 as alternative function
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
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);

    // Enable RTC
    __HAL_RCC_RTC_ENABLE();

    // RTC Time and Date
    RTC_TimeTypeDef rtc_time;
    RTC_DateTypeDef trc_date;

    while (1) {
        // print Time every 3 seconds
	    HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
	    HAL_RTC_GetDate(&hrtc, &trc_date, RTC_FORMAT_BIN);
	    printf("Time is %02u:%02u:%02u\n", (unsigned int)rtc_time.Hours, (unsigned int)rtc_time.Minutes, (unsigned int)rtc_time.Seconds);
	    HAL_Delay(3000);
    }
}

