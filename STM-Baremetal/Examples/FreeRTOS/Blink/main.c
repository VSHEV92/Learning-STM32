#include <stm32f0xx.h>

#include <FreeRTOS.h>
#include <task.h>

void blink_thread(void *argument);

void main() {

    HAL_Init();

    xTaskCreate(
        blink_thread,     // called function
        "blink_task",     // human readable name
        256,              // stack size
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
	    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
        vTaskDelay( pdMS_TO_TICKS(500) );
    }
}
