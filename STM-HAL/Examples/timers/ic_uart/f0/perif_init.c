#include "ic_uart.h"
#include "math.h"

TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart2;

void print_value(uint32_t captured_value) {

    char msg[] = "Captured value is ";
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, sizeof(msg)-1, HAL_MAX_DELAY);

    const int number_of_digits = 3;
    int base = pow(10, number_of_digits);
    int value = captured_value;
    uint8_t digit;

    for (int i = 0; i < number_of_digits; i++) {
        value %= base;
        base /= 10;

        digit = value / base + '0';

        HAL_UART_Transmit(&huart2, &digit, 1, HAL_MAX_DELAY);
    }

    uint8_t eol = '\n';
    HAL_UART_Transmit(&huart2, &eol, 1, HAL_MAX_DELAY);
}

void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim2);
}

void Peripheral_Init() {

    // Enable Clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    // Initialize GPIO pin
    GPIO_InitTypeDef GPIO_Init_Uart = {
        .Pin       = GPIO_PIN_2|GPIO_PIN_15,     // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_HIGH,       // set slew rate 
        .Alternate = GPIO_AF1_USART2,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOA, &GPIO_Init_Uart);

    GPIO_InitTypeDef GPIO_Init_Timer = {
        .Pin       = GPIO_PIN_0,             // pin number
        .Mode      = GPIO_MODE_AF_PP,        // alternative function push-pull mode
        .Pull      = GPIO_PULLDOWN,          // enable pull down registers
        .Speed     = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
        .Alternate = GPIO_AF2_TIM2,          // conncet gpio ti Timer 2
    };
    HAL_GPIO_Init(GPIOA, &GPIO_Init_Timer);

    // Initialize UART
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart2);
    
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


