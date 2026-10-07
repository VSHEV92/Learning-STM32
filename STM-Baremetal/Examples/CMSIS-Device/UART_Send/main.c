#include <stm32f0xx.h>

#define WRITE_REG_FIELD(reg, field, val) reg = ( (reg & ~field) | ( (val << (field ## _Pos)) & field ) );

#define GPIO_ALT_FUNC_MODE 0b10 
#define GPIO_NO_PULLUP_DOWN 0b00

#define UART_CLOCK SystemCoreClock
#define UART_BAUDRATE 9600


char UART_String[] = "Hello STM32F0\n";


void GPIO_Init() {
    
    // Enable GPIO clock
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    // Set as alternate function
    WRITE_REG_FIELD(GPIOA->MODER, GPIO_MODER_MODER2,  GPIO_ALT_FUNC_MODE);
    WRITE_REG_FIELD(GPIOA->MODER, GPIO_MODER_MODER15, GPIO_ALT_FUNC_MODE);

    // Set as push/pull
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_2;
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_15;

    // Disable pullup/pulldown 
    WRITE_REG_FIELD(GPIOA->PUPDR, GPIO_PUPDR_PUPDR2, GPIO_NO_PULLUP_DOWN);
    WRITE_REG_FIELD(GPIOA->PUPDR, GPIO_PUPDR_PUPDR15, GPIO_NO_PULLUP_DOWN);

    // Set alternate function number
    WRITE_REG_FIELD(GPIOA->AFR[0], GPIO_AFRL_AFSEL2, 1);
    WRITE_REG_FIELD(GPIOA->AFR[1], GPIO_AFRH_AFSEL15, 1);
}


void UART_Init() {

    // Enable UART clock
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    
    // Set baudrate
    USART2->BRR = SystemCoreClock / 9600;

    // Enable UART
    USART2->CR1 |= USART_CR1_UE;

    // Enable UART TX channel
    USART2->CR1 |= USART_CR1_TE;
}

void SysTick_Init() {
    // Set Sleep-On-Exit bit
    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;

    // Configure SysTick to fire every second
    SysTick_Config( SystemCoreClock );
}


// SysTick Handler
void SysTick_Handler() {
    char* send_char = UART_String;

    // Send all characters to TX buffer
    while(*send_char) {
        // Check that TX buffer is empty
        if (USART2->ISR & USART_ISR_TXE) {
            USART2->TDR = *send_char;
            send_char++;
        }
    }
}

void main() {
    GPIO_Init();
    UART_Init();
    SysTick_Init();

    while(1) {
    }
}
