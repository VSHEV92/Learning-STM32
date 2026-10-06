#include <stm32f0xx.h>

#define LED_PIN 3 
#define DELAY 400000

void main() {
    // Enable GPIO Port B clock
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    // Set LED Pin as output
    WRITE_REG(GPIOB->MODER, 0b01 << GPIO_MODER_MODER3_Pos);

    // Set LED Pin as push/pull
    WRITE_REG(GPIOB->OTYPER, 0b0 << 3);

    // Disable pullup/pulldown for LED Pin 
    WRITE_REG(GPIOB->PUPDR, 0b00 << GPIO_PUPDR_PUPDR3_Pos);

    // Toggle LED
    while(1) {
        WRITE_REG(GPIOB->ODR, 0b1 << LED_PIN);
        for(volatile int i = 0; i < DELAY; i++) {}

        WRITE_REG(GPIOB->ODR, 0b0 << LED_PIN);
        for(volatile int i = 0; i < DELAY; i++) {}
    }


    
}
