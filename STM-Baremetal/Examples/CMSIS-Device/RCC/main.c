#include <stm32f0xx.h>

#define WRITE_REG_FIELD(reg, field, val) reg = ( (reg & ~field) | ( (val << (field ## _Pos)) & field ) );

#define GPIO_ALT_FUNC_MODE  0b10 
#define GPIO_NO_PULLUP_DOWN 0b00

#define RCC_PLL_PREDIV_4   0b0011
#define RCC_PLL_HSI_SOURCE   0b01
#define RCC_PLL_MUL_16     0b1110
#define RCC_APB_PRE_4       0b101

#define UART_CLOCK    8000000
#define UART_BAUDRATE 9600


char UART_String[] = "Hello STM32F0\n";

void RCC_Init() {
    // Configure PLL Clock to 32 MHz
    
    // 1. We plan to set System Clock to 32 MHz
    //    Is clock frequency more then 24 MHz we need adjust flash latency
    FLASH->ACR |= FLASH_ACR_LATENCY;

    // 2. Set PLL PREDIV value to 4
    WRITE_REG_FIELD(RCC->CFGR2, RCC_CFGR2_PREDIV, RCC_PLL_PREDIV_4);

    // 3. Choose HSI/PREDIV as PLL Source
    //    PLL input frequency is 2 MHz
    WRITE_REG_FIELD(RCC->CFGR, RCC_CFGR_PLLSRC, RCC_PLL_HSI_SOURCE);

    // 4. Set PLL Multiplication to 16
    //    PLL Output will be 32 MHz
    WRITE_REG_FIELD(RCC->CFGR, RCC_CFGR_PLLMUL, RCC_PLL_MUL_16);

    // 5. Enable PLL and wait until it's ready
    RCC->CR |= RCC_CR_PLLON;
    while ( !(RCC->CR & RCC_CR_PLLRDY) ) {}

    // 6. Switch System clock to PLL
    WRITE_REG_FIELD(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);

    // 7. Wait until System Clock switched to PLL
    while ( (RCC->CFGR & RCC_CFGR_SWS) !=  RCC_CFGR_SWS_PLL) {}

    // 8. Update SystemCoreClock variable
    SystemCoreClockUpdate();

    // 9. Set APB clock Divider to 4
    //    So UART2 Clock will be 8 MHz
    WRITE_REG_FIELD(RCC->CFGR, RCC_CFGR_PPRE, RCC_APB_PRE_4);

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
    USART2->BRR = UART_CLOCK / UART_BAUDRATE;

    // Enable UART
    USART2->CR1 |= USART_CR1_UE;

    // Enable UART TX channel
    USART2->CR1 |= USART_CR1_TE;
}

void SysTick_Init() {
    // Set Sleep-On-Exit bit
    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;

    // Configure SysTick to fire every 500 ms
    SysTick_Config( SystemCoreClock / 2 );
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
    RCC_Init();
    GPIO_Init();
    UART_Init();
    SysTick_Init();

    while(1) {
    }
}
