#include "native_timer.h"


void blink_task_func(void *argument);
void hello_print_timer_cb(TimerHandle_t xTimer);

TimerHandle_t autoreload_timer;


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

    xTaskCreate(
        blink_task_func,  // called function
        "Blink Task",     // human readable name
        1024,             // stack size
        NULL,             // task function parameter (void*)
        1,                // task priority
        NULL              // task handler
    );

    autoreload_timer = xTimerCreate (
        "Hello timer",            // human readable timer name
        pdMS_TO_TICKS( 1000 ),    // timer period in ticks
        pdTRUE,                   // enable autoreload
        NULL,                     // set timer ID
        hello_print_timer_cb      // timer callback function 
    );

    xTimerStart(
        autoreload_timer,     // timer handler
        portMAX_DELAY         // max delay for waiting until command send to deamon command queue
    );

    vTaskStartScheduler();

    while (1) {}

}

// Blink Thread Function
void blink_task_func(void *argument) {
    for(;;) {
	    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        vTaskDelay( pdMS_TO_TICKS(500) );
    }
}

// Timer Callback
void hello_print_timer_cb(TimerHandle_t xTimer) {
    printf("Hello from timer\n");
}
