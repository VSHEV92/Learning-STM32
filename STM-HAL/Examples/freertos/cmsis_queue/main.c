#include "cmsis_queue.h"



// TX Thread
osThreadId_t sendTaskHandle;
const osThreadAttr_t sendTask_attributes = {
    .name = "sendTask",
    .stack_size = 1024 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};
void sendTask_func(void *argument);

// RX Thread
osThreadId_t receiveTaskHandle;
const osThreadAttr_t receiveTask_attributes = {
    .name = "receiveTask",
    .stack_size = 1024 * 4,
    .priority = (osPriority_t) osPriorityAboveNormal,
};
void receiveTask_func(void *argument);

// Queue
osMessageQueueId_t queueHandle;
const osMessageQueueAttr_t queue_attributes = {
    .name = "queue"
};


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

    // Create Queue
    queueHandle = osMessageQueueNew (16, sizeof(uint8_t), &queue_attributes);

    // Create TX thread
    sendTaskHandle = osThreadNew(sendTask_func, NULL, &sendTask_attributes);

    // Create RX thread
    receiveTaskHandle = osThreadNew(receiveTask_func, NULL, &receiveTask_attributes);

    osKernelStart();

    while (1) {}

}


// TX Thread Function
void sendTask_func(void *argument) {

    uint8_t counter = 0;
    for(;;) {
	    printf("Put counter value %u\n", (unsigned int)counter);
	    osMessageQueuePut(queueHandle, &counter, 0U, osWaitForever);
        osDelay(1000);
        counter++;
    }

}


// RX Thread Function
void receiveTask_func(void *argument) {
    uint8_t counter;
    for(;;) {
	    osMessageQueueGet(queueHandle, &counter, 0U, osWaitForever);
	    printf("Get counter value %u\n", (unsigned int)counter);
    }
}

