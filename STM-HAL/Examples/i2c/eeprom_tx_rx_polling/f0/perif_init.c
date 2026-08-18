#include "eeprom_tx_rx_polling.h"
#include "math.h"

void print_int(int value, int number_of_digits) {

    int base = pow(10, number_of_digits);
    uint8_t digit;

    for (int i = 0; i < number_of_digits; i++) {
        value %= base;
        base /= 10;

        digit = value / base + '0';

        HAL_UART_Transmit(&huart2, &digit, 1, HAL_MAX_DELAY);
    }

}

// Peripheral Handlers
UART_HandleTypeDef huart2;
I2C_HandleTypeDef hi2c1;

void Peripheral_Init() {
    
    // Enable clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_I2C1_CLK_ENABLE();


    // Setup GPIO pins
    GPIO_InitTypeDef GPIO_Init_UART = {
        .Pin       = GPIO_PIN_2|GPIO_PIN_15,     // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_HIGH,       // set slew rate 
        .Alternate = GPIO_AF1_USART2,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOA, &GPIO_Init_UART);

    GPIO_InitTypeDef GPIO_Init_I2C = {
        .Pin       = GPIO_PIN_6|GPIO_PIN_7,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_OD,            // set pins mode to alternative function with open drain
        .Pull      = GPIO_PULLUP,                // enable pull up registers
        .Speed     = GPIO_SPEED_FREQ_LOW,        // set slew rate 
        .Alternate = GPIO_AF1_I2C1,              // choose I2C1 as alternative function
    };
    HAL_GPIO_Init(GPIOB, &GPIO_Init_I2C);


    // Initalize UART
    huart2.Instance          = USART2;                 // uart interface instance
    huart2.Init.BaudRate     = 9600;                   // buad rate
    huart2.Init.WordLength   = UART_WORDLENGTH_8B;     // bits per symbol
    huart2.Init.StopBits     = UART_STOPBITS_1;        // 1 stop bit
    huart2.Init.Parity       = UART_PARITY_NONE;       // don't use parity bit
    huart2.Init.Mode         = UART_MODE_TX;           // enable uart tx 
    huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;    // don't use hardware flow control
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;   // set oversampling to 16 bits  
    HAL_UART_Init(&huart2);

    // Initialize I2C
    hi2c1.Instance             = I2C1;                    // I2C instance 
    hi2c1.Init.Timing          = 0x2000090E;              // Timong value
    hi2c1.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT; // Address mode is 7 bits
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; // Disable dual address mode
    hi2c1.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;   // Disable clock streching
    HAL_I2C_Init(&hi2c1);

}

