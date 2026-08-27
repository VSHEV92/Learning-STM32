#include "cmsis_blink.h"


// Blink Thread
const osThreadAttr_t thread_attributes = {
    .name = "blink_task",
    .stack_size = 256 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};
void blink_thread(void *argument);


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

    osThreadNew(blink_thread, NULL, &thread_attributes);
    osKernelStart();

    while (1) {}

}


// Blink Thread Function
void blink_thread(void *argument) {
    for(;;) {
	    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        osDelay(500);
    }
}

