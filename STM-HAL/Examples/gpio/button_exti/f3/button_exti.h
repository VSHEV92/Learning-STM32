#include "stm32f3xx_hal.h"

#define LED_PORT GPIOE
#define LED_PIN GPIO_PIN_15

#define BUTTON_PORT GPIOA
#define BUTTON_PIN GPIO_PIN_0

#define EXTI_ISR EXTI0_IRQHandler

void Peripheral_Init();
