#include "native_blink.h"

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

    // Enable GPIO Port A clock
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Initialize GPIO pin
    GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin   = GPIO_PIN_5,             // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // PendSV_IRQn interrupt configuration
    HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
    

    // ------------------------------------------------------
    // --------------- Kernel Initialization ----------------
    // ------------------------------------------------------
    
    xTaskCreate(
        blink_thread,     // called function
        "blink_task",     // human readable name
        1024,             // stack size
        NULL,             // task function parameter (void*)
        1,                // task priority
        NULL              // task handler
    );

    vTaskStartScheduler();

    while (1) {}

}


// Blink Thread Function
void blink_thread(void *argument) {
    for(;;) {
	    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        vTaskDelay( pdMS_TO_TICKS(300) );
    }
}

