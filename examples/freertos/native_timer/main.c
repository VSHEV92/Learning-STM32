#include "native_timer.h"

UART_HandleTypeDef huart2;

void blink_task_func(void *argument);
void hello_print_timer_cb(TimerHandle_t xTimer);

TimerHandle_t autoreload_timer;


// Set UART 2 interface as input/output stream
int __io_putchar(int ch) {
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY); 
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

    // Enable clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();


    // Setup GPIO pins
    GPIO_InitTypeDef GPIO_Init_UART = {
        .Pin       = GPIO_PIN_2|GPIO_PIN_3,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_VERY_HIGH,  // set slew rate 
        .Alternate = GPIO_AF7_USART2,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOA, &GPIO_Init_UART);

    GPIO_InitTypeDef GPIO_Init_LED = {
        .Pin   = GPIO_PIN_5,             // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(GPIOA, &GPIO_Init_LED);


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


    // PendSV_IRQn interrupt configuration
    HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
    

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
	    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        vTaskDelay( pdMS_TO_TICKS(500) );
    }
}

// Timer Callback
void hello_print_timer_cb(TimerHandle_t xTimer) {
    printf("Hello from timer\n");
}
