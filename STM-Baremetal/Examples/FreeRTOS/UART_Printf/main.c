#include <stm32f0xx.h>

#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>

void blink_thread(void *argument);
void hello_world_thread(void *argument);

void main() {

    HAL_Init();

    xTaskCreate(
        blink_thread,     // called function
        "blink_task",     // human readable name
        64,               // stack size
        NULL,             // task function parameter (void*)
        1,                // task priority
        NULL              // task handler
    );

    xTaskCreate(
        hello_world_thread,     // called function
        "hello_task",           // human readable name
        256,                    // stack size
        NULL,                   // task function parameter (void*)
        1,                      // task priority
        NULL                    // task handler
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


// Hello World Thread Function
void hello_world_thread(void *argument) {
    for(;;) {
        printf("Hello World\n");
        vTaskDelay( pdMS_TO_TICKS(1000) );
    }
}
