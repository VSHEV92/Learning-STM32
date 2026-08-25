#include "ic_uart.h"

TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart1;

void print_value(uint32_t captured_value) {
    printf("Captured value is %ld\n", captured_value);
}

void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim2);
}

void Peripheral_Init() {

    // Enable Clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    // Initialize GPIO pin
    GPIO_InitTypeDef GPIO_Init_Uart = {
        .Pin       = GPIO_PIN_4|GPIO_PIN_5,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_HIGH,       // set slew rate 
        .Alternate = GPIO_AF7_USART1,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOC, &GPIO_Init_Uart);

    GPIO_InitTypeDef GPIO_Init_Timer = {
        .Pin       = GPIO_PIN_0,             // pin number
        .Mode      = GPIO_MODE_AF_PP,        // alternative function push-pull mode
        .Pull      = GPIO_NOPULL,            // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
        .Alternate = GPIO_AF1_TIM2,          // conncet gpio ti Timer 2
    };
    HAL_GPIO_Init(GPIOA, &GPIO_Init_Timer);

    // Initialize UART
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 9600;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart1);
    
    // Initialize Timer
    htim2.Instance               = TIM2;                            // set timer instance  
    htim2.Init.Prescaler         = 8000 - 1;                        // set timer clock prescaler
    htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;              // set counter to UP mode 
    htim2.Init.Period            = 1000 - 1;                        // set timer period
    htim2.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;          // set timer clock division of sampling clock 
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;  // set disable shadow preload register 
    HAL_TIM_Base_Init(&htim2);

    // Configure Input Capture
    TIM_IC_InitTypeDef IC_Config = {
        .ICPolarity  = TIM_INPUTCHANNELPOLARITY_RISING,  // Capture on rising edge
        .ICSelection = TIM_ICSELECTION_DIRECTTI,         // Direct connection trigger input to capture port
        .ICPrescaler = TIM_ICPSC_DIV1,                   // Capture value on every trigger event
        .ICFilter    = 0,                                // Disable input capture filter
    };
    HAL_TIM_IC_ConfigChannel(&htim2, &IC_Config, TIM_CHANNEL_1);  
    

    // Setup timer interrupts
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

}


