#include <stm32f0xx_ll_bus.h>
#include <stm32f0xx_ll_gpio.h>
#include <stm32f0xx_ll_usart.h>
#include <stm32f0xx_ll_utils.h>

#define UART_BAUDRATE 9600

char hello_string[] = "Hello STM32F0\n";

void main() {

    // Init strutctures to default values
    LL_GPIO_InitTypeDef gpio_init;
    LL_GPIO_StructInit(&gpio_init);

    LL_USART_InitTypeDef uart_init;
    LL_USART_StructInit(&uart_init);

    // Enable clock
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);


    // Configure needed values
    gpio_init.Pin = LL_GPIO_PIN_2 | LL_GPIO_PIN_15;
    gpio_init.Mode = LL_GPIO_MODE_ALTERNATE;          
    gpio_init.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpio_init.Pull = LL_GPIO_PULL_NO;              
    gpio_init.Alternate = LL_GPIO_AF_1;              
    
    LL_GPIO_Init (GPIOA, &gpio_init);
    

    // Configure UART
    uart_init.BaudRate = UART_BAUDRATE;
    uart_init.DataWidth = LL_USART_DATAWIDTH_8B;
    uart_init.StopBits = LL_USART_STOPBITS_1;
    uart_init.Parity = LL_USART_PARITY_NONE;
    uart_init.TransferDirection = LL_USART_DIRECTION_TX;
    uart_init.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    uart_init.OverSampling = LL_USART_OVERSAMPLING_16;

    LL_USART_Init (USART2, &uart_init);
    LL_USART_Enable(USART2);


    // Init SysTick to 1 ms period
    LL_Init1msTick(SystemCoreClock);
    

    while(1) {
        char *c = hello_string;
        while(*c) {
            if ( LL_USART_IsActiveFlag_TXE(USART2) ) {
                LL_USART_TransmitData8(USART2, *c);
                c++;
            }
        }
        LL_mDelay(500);
    }
    
}
