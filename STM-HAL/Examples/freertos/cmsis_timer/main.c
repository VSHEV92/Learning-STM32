#include "cmsis_timer.h"

// Blink Thread
osThreadId_t blink_taskHandle;
const osThreadAttr_t blink_task_attributes = {
    .name = "blink_task",
    .stack_size = 1024 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};
void blink_task_func(void *argument);


// Timer 
osTimerId_t hello_print_timerHandle;
const osTimerAttr_t hello_print_timer_attributes = {
    .name = "hello_print_timer"
};
void hello_print_timer_cb(void *argument);


// Set UART 2 interface as input/output stream
int __io_putchar(int ch) {
  HAL_UART_Transmit(&STDIO_UART, (uint8_t *)&ch, 1, HAL_MAX_DELAY); 
  return ch;
}


void main() {

    // ------------------------------------------------------
    // ------------- Hardware Initialization ----------------
    // ------------------------------------------------------

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

    

    // ------------------------------------------------------
    // --------------- Kernel Initialization ----------------
    // ------------------------------------------------------

    osKernelInitialize();

    // Create and Start Timer
    hello_print_timerHandle = osTimerNew(hello_print_timer_cb, osTimerPeriodic, NULL, &hello_print_timer_attributes);
    osTimerStart(hello_print_timerHandle, 1000);
  
    // Create Blink Thread
    blink_taskHandle = osThreadNew(blink_task_func, NULL, &blink_task_attributes);

    osKernelStart();

    while (1) {}

}


// Blink Thread Function
void blink_task_func(void *argument) {
    for(;;) {
	    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        osDelay(500);
    }
}

// Timer Callback
void hello_print_timer_cb(void *argument) {
    printf("Hello from timer\n");
}
