#include "rtc.h"

// Set UART interface as input/output stream
int __io_putchar(int ch) {
  HAL_UART_Transmit(&STDIO_UART, (uint8_t *)&ch, 1, HAL_MAX_DELAY); 
  return ch;
}

// RTC ISR Handler
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) {
    printf("Alarm callback is triggered\n");
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
	    printf("Time is %02u:%02u:%02u\n", (unsigned int)rtc_time.Hours, (unsigned int)rtc_time.Minutes, (unsigned int)rtc_time.Seconds);
	    HAL_Delay(3000);
    }
}

