#include "eeprom_tx_rx_polling.h"

#define EEPROM_ADDR 0xA0
#define COUNTER_ADDR 0x00

// Peripheral Handlers
UART_HandleTypeDef huart2;
I2C_HandleTypeDef hi2c1;

// Set UART 2 interface as input/output stream
int __io_putchar(int ch) {
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY); 
  return ch;
}

void main() {

    /*
     * 1. Enable instruction and data cache. Enable instruction prefetch.
     * 2. Set NVIC priority grouping to 4 bits 
     * 3. Call HAL_InitTick(), which:
     *    - set Systick to 1ms period
     *    - set Systick IRQ priority to TICK_INT_PRIORITY (minimum).
     */
    HAL_Init();

    
    // Enable clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_I2C1_CLK_ENABLE();


    // Setup GPIO pins
    GPIO_InitTypeDef GPIO_Init_UART = {
        .Pin       = GPIO_PIN_2|GPIO_PIN_3,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_PP,            // set pins mode to alternative function
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_VERY_HIGH,  // set slew rate 
        .Alternate = GPIO_AF7_USART2,            // choose UART2 as alternative function
    };
    HAL_GPIO_Init(GPIOA, &GPIO_Init_UART);

    GPIO_InitTypeDef GPIO_Init_I2C = {
        .Pin       = GPIO_PIN_8|GPIO_PIN_9,      // choose GPIO pins
        .Mode      = GPIO_MODE_AF_OD,            // set pins mode to alternative function with open drain
        .Pull      = GPIO_NOPULL,                // disable pull up/down registers
        .Speed     = GPIO_SPEED_FREQ_VERY_HIGH,  // set slew rate 
        .Alternate = GPIO_AF4_I2C1,              // choose I2C1 as alternative function
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
    hi2c1.Init.ClockSpeed      = 100000;                  // SCK frequency
    hi2c1.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT; // Address mode is 7 bits
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; // Disable dual address mode
    hi2c1.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;   // Disable clock streching
    HAL_I2C_Init(&hi2c1);


    // Write data to EEPROM and read int back
    uint8_t counter;
    uint8_t tx_data[2] = {COUNTER_ADDR, 0};

    while (1) {
        // set address inside EEPROM
	    HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_ADDR, 1, HAL_MAX_DELAY);
        HAL_I2C_Master_Transmit(&hi2c1, EEPROM_ADDR, tx_data, 1, HAL_MAX_DELAY);

        // read counter value
	    HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_ADDR, 1, HAL_MAX_DELAY);
	    HAL_I2C_Master_Receive(&hi2c1, EEPROM_ADDR, &counter, 1, HAL_MAX_DELAY);
        printf("Counter value from EEPROM: %d\n", (int)counter);

        // incremetn counter and write it back
        tx_data[1] = ++counter;
        HAL_I2C_Master_Transmit(&hi2c1, EEPROM_ADDR, tx_data, 2, HAL_MAX_DELAY);

        // wait 1 second and start again
        HAL_Delay(1000);
    }
}

