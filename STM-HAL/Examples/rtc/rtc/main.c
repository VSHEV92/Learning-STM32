#include "rtc.h"

// Set UART interface as input/output stream
int __io_putchar(int ch) {
  HAL_UART_Transmit(&STDIO_UART, (uint8_t *)&ch, 1, HAL_MAX_DELAY); 
  return ch;
}

// RTC ISR Handler
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) {
    uint8_t msg[] = "Alarm callback is triggered\n";
    HAL_UART_Transmit(&STDIO_UART, msg, sizeof(msg) - 1, HAL_MAX_DELAY); 
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


    // RTC Time and Date
    RTC_TimeTypeDef rtc_time;
    RTC_DateTypeDef trc_date;

    while (1) {
        // print Time every 3 seconds
	    HAL_RTC_GetTime(&EXAMPLE_RTC, &rtc_time, RTC_FORMAT_BIN);
	    HAL_RTC_GetDate(&EXAMPLE_RTC, &trc_date, RTC_FORMAT_BIN);

    #ifndef STM32F042x6
	    printf("Time is %02u:%02u:%02u\n", (unsigned int)rtc_time.Hours, (unsigned int)rtc_time.Minutes, (unsigned int)rtc_time.Seconds);
    #else

        char msg[] = "Time is ";

        uint8_t colon = ':';
        uint8_t eol = '\n';

        HAL_UART_Transmit(&huart2, (uint8_t*)msg, sizeof(msg)-1, HAL_MAX_DELAY);

        print_int(rtc_time.Hours, 2);
        HAL_UART_Transmit(&huart2, &colon, 1, HAL_MAX_DELAY);

        print_int(rtc_time.Minutes, 2);
        HAL_UART_Transmit(&huart2, &colon, 1, HAL_MAX_DELAY);

        print_int(rtc_time.Seconds, 2);

        HAL_UART_Transmit(&huart2, &eol, 1, HAL_MAX_DELAY);

    #endif
        HAL_Delay(3000);
    }
}

