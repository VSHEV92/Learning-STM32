#include "stm32h7xx_hal.h"

#define LED_PORT GPIOA
#define LED_PIN GPIO_PIN_5

#define BUTTON_PORT GPIOC
#define BUTTON_PIN GPIO_PIN_0

#define EXTI_ISR EXTI0_IRQHandler

void Peripheral_Init();
