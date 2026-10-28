#include <stm32f0xx.h>

#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>

void blink_thread(void *argument);
void hello_world_thread(void *argument);
void notified_thread(void *argument);
void timer_cb(TimerHandle_t xTimer);

TimerHandle_t autoreload_timer;
SemaphoreHandle_t semaphore;

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
        128,                    // stack size
        NULL,                   // task function parameter (void*)
        1,                      // task priority
        NULL                    // task handler
    );

    xTaskCreate(
        notified_thread,        // called function
        "notified_task",        // human readable name
        128,                    // stack size
        NULL,                   // task function parameter (void*)
        2,                      // task priority
        NULL                    // task handler
    );

    autoreload_timer = xTimerCreate (
        "Timer",                  // human readable timer name
        pdMS_TO_TICKS( 301 ),     // timer period in ticks
        pdTRUE,                   // enable autoreload
        NULL,                     // set timer ID
        timer_cb                  // timer callback function 
    );

    xTimerStart(
        autoreload_timer,     // timer handler
        portMAX_DELAY         // max delay for waiting until command send to deamon command queue
    );

    semaphore = xSemaphoreCreateBinary();

    vTaskStartScheduler();

    while (1) {}

}


// Blink Thread Function
void blink_thread(void *argument) {
    for(;;) {
	    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
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


// Notified Thread Function
void notified_thread(void *argument) {
    for(;;) {
        xSemaphoreTake(semaphore, portMAX_DELAY);
        printf("Task was notified\n");
    }
}

// Timer Callback
void timer_cb(TimerHandle_t xTimer) {
    printf("Hello from timer\n");
}
