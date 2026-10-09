#include <stm32f0xx.h>
#include <stdio.h>

#define WRITE_REG_FIELD(reg, field, val) reg = ( (reg & ~field) | ( (val << (field ## _Pos)) & field ) );

#define GPIO_ALT_FUNC_MODE 0b10 
#define GPIO_NO_PULLUP_DOWN 0b00

#define UART_CLOCK SystemCoreClock
#define UART_BAUDRATE 9600

// Redirect stdlib stream to UART
int _write(int file, char *ptr, int len)  {
    for (int i = 0; i < len; i++) {
        // Wait for empty TX buffer and send character
        while ( !(USART2->ISR & USART_ISR_TXE) ) {}
        USART2->TDR = ptr[i];
    }
    return len;
}


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

void TIM14_Init() {
    // Enable TIM14 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;

    // Set Prescaler Register to 8000 - 1
    // So we divide APB clock by 8000
    TIM14->PSC = 8000 - 1;

    // Set Reload value to 1000 - 1
    // So timer will by fired every 250 ms
    TIM14->ARR = 250 - 1;

    // Enable Update Interrupt
    TIM14->DIER |= TIM_DIER_UIE;

    // Enable Interrupts in NVIC
    NVIC_EnableIRQ(TIM14_IRQn);

    // Start timer
    TIM14->CR1 |= TIM_CR1_CEN;
}


uint8_t counter = 0;

// TIM14 Handler
void TIM14_IRQHandler() {
    printf("Counter: %d\n", counter++);

    // Clear pending flag
    TIM14->SR = ~TIM_SR_UIF;
}

void main() {
    GPIO_Init();
    UART_Init();
    TIM14_Init();

    while(1) {
    }
}
