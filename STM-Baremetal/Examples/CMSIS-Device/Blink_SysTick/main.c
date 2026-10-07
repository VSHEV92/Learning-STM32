#include <stm32f0xx.h>

#define WRITE_REG_FIELD(reg, field, val) reg = ( (reg & ~field) | ( (val << (field ## _Pos)) & field ) );

#define GPIO_OUTPUT_MODE 0b01 
#define GPIO_NO_PULLUP_DOWN 0b00


// SysTick Handler
void SysTick_Handler() {
    GPIOB->ODR ^= GPIO_ODR_3;
}


void main() {
    // Enable GPIO Port B clock
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    // Set LED Pin as output
    WRITE_REG_FIELD(GPIOB->MODER, GPIO_MODER_MODER3, GPIO_OUTPUT_MODE);

    // Set LED Pin as push/pull
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT_3;

    // Disable pullup/pulldown for LED Pin 
    WRITE_REG_FIELD(GPIOB->PUPDR, GPIO_PUPDR_PUPDR3, GPIO_NO_PULLUP_DOWN);

    // Configure SysTick to fire every 500 ms
    SysTick_Config( SystemCoreClock/2 );
    
    // Set Sleep-On-Exit bit
    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;

    // Wait for interrupt
    __WFI();
    
}
