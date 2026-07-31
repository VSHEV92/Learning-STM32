#include "native_queue.h"

void sendTask_func(void *argument);
void receiveTask_func(void *argument);

QueueHandle_t queue;


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
    
    queue = xQueueCreate( 
        5,               // number of items in queue                   
        sizeof(uint32_t) // size of one item in bytes
    ); 


    xTaskCreate(
        sendTask_func,    // called function
        "TX Task",        // human readable name
        1024,             // stack size
        NULL,             // task function parameter (void*)
        1,                // task priority
        NULL              // task handler
    );
    
    xTaskCreate(
        receiveTask_func, // called function
        "RX Task",        // human readable name
        1024,             // stack size
        NULL,             // task function parameter (void*)
        1,                // task priority
        NULL              // task handler
    );

    vTaskStartScheduler();

    while (1) {}

}


// TX Thread Function
void sendTask_func(void *argument) {

    uint8_t counter = 0;
    for(;;) {
	    printf("Put counter value %u\n", (unsigned int)counter);
        xQueueSendToBack(queue, (void*)&counter, portMAX_DELAY);
        vTaskDelay( pdMS_TO_TICKS(1000) );
        counter++;
    }

}


// RX Thread Function
void receiveTask_func(void *argument) {
    uint8_t counter;
    for(;;) {
        xQueueReceive(queue, &counter, portMAX_DELAY);
	    printf("Get counter value %u\n", (unsigned int)counter);
    }
}

