#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stm32f0xx.h>

#define configUSE_16_BIT_TICKS                   0

#define configUSE_PREEMPTION                     1
#define configSUPPORT_DYNAMIC_ALLOCATION         1
#define configENABLE_MPU                         0
#define configCHECK_HANDLER_INSTALLATION         0

#define configMAX_PRIORITIES                     ( 4 )
#define configMINIMAL_STACK_SIZE                 ((uint16_t)128)
#define configTOTAL_HEAP_SIZE                    ((size_t)3072)
#define configCPU_CLOCK_HZ                       ( SystemCoreClock )
#define configTICK_RATE_HZ                       ((TickType_t)1000)

#define configUSE_IDLE_HOOK                      0
#define configUSE_TICK_HOOK                      0


#define configUSE_TIMERS                         1
#define configTIMER_TASK_PRIORITY                ( 2 )
#define configTIMER_QUEUE_LENGTH                 1
#define configTIMER_TASK_STACK_DEPTH             128


#define INCLUDE_vTaskDelay                      1

#endif
