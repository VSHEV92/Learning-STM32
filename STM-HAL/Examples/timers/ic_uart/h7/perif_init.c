#include "ic_uart.h"

TIM_HandleTypeDef htim3;
UART_HandleTypeDef huart3;


void TIM3_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim3);
}

void Peripheral_Init() {

    // Enable Clocks
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_TIM3_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();

    // Initialize GPIO pin
    GPIO_InitTypeDef GPIO_Init_Uart = {
        .Pin       = GPIO_PIN_8|GPIO_PIN_9,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_VERY_HIGH,  // set slew rate 
        .Alternate = GPIO_AF7_USART3,            // choose UART3 as alternative function
    };
    HAL_GPIO_Init(GPIOD, &GPIO_Init_Uart);

    GPIO_InitTypeDef GPIO_Init_Timer = {
        .Pin       = GPIO_PIN_1,             // pin number
        .Mode      = GPIO_MODE_AF_PP,        // alternative function push-pull mode
        .Pull      = GPIO_NOPULL,            // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
        .Alternate = GPIO_AF2_TIM3,          // conncet gpio ti Timer 3
    };
    HAL_GPIO_Init(GPIOB, &GPIO_Init_Timer);

    // Initialize UART
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 9600;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart3);
    
    // Initialize Timer
    htim3.Instance               = TIM3;                            // set timer instance  
    htim3.Init.Prescaler         = 64000 - 1;                       // set timer clock prescaler
    htim3.Init.CounterMode       = TIM_COUNTERMODE_UP;              // set counter to UP mode 
    htim3.Init.Period            = 1000 - 1;                        // set timer period
    htim3.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;          // set timer clock division of sampling clock 
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;  // set disable shadow preload register 
    HAL_TIM_Base_Init(&htim3);

    // Configure Input Capture
    TIM_IC_InitTypeDef IC_Config = {
        .ICPolarity  = TIM_INPUTCHANNELPOLARITY_RISING,  // Capture on rising edge
        .ICSelection = TIM_ICSELECTION_DIRECTTI,         // Direct connection trigger input to capture port
        .ICPrescaler = TIM_ICPSC_DIV1,                   // Capture value on every trigger event
        .ICFilter    = 0,                                // Disable input capture filter
    };
    HAL_TIM_IC_ConfigChannel(&htim3, &IC_Config, TIM_CHANNEL_4);  
    

    // Setup timer interrupts
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);

}


