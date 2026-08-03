#include "eeprom_seq_interrupt.h"

// Peripheral Handlers
UART_HandleTypeDef huart3;
I2C_HandleTypeDef hi2c1;


void I2C1_EV_IRQHandler(void) { 
    HAL_I2C_EV_IRQHandler(&hi2c1);
}

void Peripheral_Init() {
    
    // Enable clocks
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    __HAL_RCC_USART3_CLK_ENABLE();
    __HAL_RCC_I2C1_CLK_ENABLE();


    // Setup GPIO pins
    GPIO_InitTypeDef GPIO_Init_UART = {
        .Pin       = GPIO_PIN_8|GPIO_PIN_9,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_VERY_HIGH,  // set slew rate 
        .Alternate = GPIO_AF7_USART3,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOD, &GPIO_Init_UART);

    GPIO_InitTypeDef GPIO_Init_I2C = {
        .Pin       = GPIO_PIN_8|GPIO_PIN_9,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_OD,            // set pins mode to alternative function with open drain
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_VERY_HIGH,  // set slew rate 
        .Alternate = GPIO_AF4_I2C1,              // choose I2C1 as alternative function
    };
    HAL_GPIO_Init(GPIOB, &GPIO_Init_I2C);


    // Initalize UART
    huart3.Instance          = USART3;                 // uart interface instance
    huart3.Init.BaudRate     = 9600;                   // buad rate
    huart3.Init.WordLength   = UART_WORDLENGTH_8B;     // bits per symbol
    huart3.Init.StopBits     = UART_STOPBITS_1;        // 1 stop bit
    huart3.Init.Parity       = UART_PARITY_NONE;       // don't use parity bit
    huart3.Init.Mode         = UART_MODE_TX;           // enable uart tx 
    huart3.Init.HwFlowCtl    = UART_HWCONTROL_NONE;    // don't use hardware flow control
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;   // set oversampling to 16 bits  
    HAL_UART_Init(&huart3);

    // Initialize I2C
    hi2c1.Instance             = I2C1;                    // I2C instance 
    hi2c1.Init.Timing          = 0x10707DBC;
    hi2c1.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT; // Address mode is 7 bits
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; // Disable dual address mode
    hi2c1.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;   // Disable clock streching
    HAL_I2C_Init(&hi2c1);

    // I2C interrupts enable
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);

}

