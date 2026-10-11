#include <stm32f0xx.h>
#include <stdlib.h>
#include <stdio.h>

#define WRITE_REG_FIELD(reg, field, val) reg = ( (reg & ~field) | ( (val << (field ## _Pos)) & field ) );

#define GPIO_ALT_FUNC_MODE 0b10 
#define GPIO_NO_PULLUP_DOWN 0b00

#define UART_CLOCK SystemCoreClock
#define UART_BAUDRATE 9600

// DMA RX and TX data
int tx_data[] = {1, 2, 3, 4, 5};
int* rx_data;


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


void DMA_Init() {
    
    // Enable DMA clock
    RCC->AHBENR |= RCC_AHBENR_DMAEN;

    // Disable DMA
    DMA1_Channel4->CCR &= ~DMA_CCR_EN;


    // Set Transfer Sizes to 32 bits
    WRITE_REG_FIELD(DMA1_Channel4->CCR, DMA_CCR_PSIZE, 0b10);
    WRITE_REG_FIELD(DMA1_Channel4->CCR, DMA_CCR_MSIZE, 0b10);

    // Set Direction for Memory to Peripheral
    WRITE_REG_FIELD(DMA1_Channel4->CCR, DMA_CCR_DIR, 1);

    // Set Mem2Mem flag
    WRITE_REG_FIELD(DMA1_Channel4->CCR, DMA_CCR_MEM2MEM, 1);

    // Increment all addresses
    WRITE_REG_FIELD(DMA1_Channel4->CCR, DMA_CCR_MINC, 1);
    WRITE_REG_FIELD(DMA1_Channel4->CCR, DMA_CCR_PINC, 1);

}


void UART_Init() {

    // Enable UART clock
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    
    // Set baudrate
    USART2->BRR = SystemCoreClock / UART_BAUDRATE;

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

    // Get memory from Heap for rx_data
    rx_data = calloc( sizeof(tx_data)/sizeof(tx_data[0]), sizeof(tx_data[0]) );

    // Disable DMA
    DMA1_Channel4->CCR &= ~DMA_CCR_EN;

    // Set Source Addess
    DMA1_Channel4->CMAR = (uint32_t)tx_data;
    
    // Set Destination Adderss
    DMA1_Channel4->CPAR = (uint32_t)rx_data;

    // Set Transer size to string Length
    DMA1_Channel4->CNDTR = sizeof(tx_data) / sizeof(tx_data[0]);

    // Enable Transfer completion interrupt in DMA
    DMA1_Channel4->CCR |= DMA_CCR_TCIE;

    // Enable Interrupts in NVIC
    NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);

    // Start Transfer
    DMA1_Channel4->CCR |= DMA_CCR_EN;
}


// DMA completion channel handler
void DMA1_Channel4_5_IRQHandler() {
    
    // Print Received Data
    printf("Receiver data: ");
    for (int i = 0; i < sizeof(tx_data) / sizeof(tx_data[0]); i++) {
        printf("%d ", rx_data[i]);
    }
    printf("\n");

    // Clean Transfer Completion Interrupt Flag
    DMA1->IFCR = DMA_IFCR_CTCIF4;

    // Free Memory
    free(rx_data);
}


void main() {
    GPIO_Init();
    UART_Init();
    DMA_Init();
    SysTick_Init();

    while(1) {
    }
}
