#include "stm32f4xx_hal.h"

#define LED_PORT GPIOA
#define LED_PIN GPIO_PIN_5

#define BUTTON_PORT GPIOA
#define BUTTON_PIN GPIO_PIN_1

#define EXTI_ISR EXTI1_IRQHandler

void Peripheral_Init();
