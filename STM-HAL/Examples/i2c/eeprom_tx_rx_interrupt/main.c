#include "eeprom_tx_rx_interrupt.h"

#define EEPROM_ADDR 0xA0
#define COUNTER_ADDR 0x00

// I2C Interrupt Handlers
volatile uint32_t tx_completed = 0;
volatile uint32_t rx_completed = 0;

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	tx_completed = 1;
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	rx_completed = 1;
}

// Set UART 2 interface as input/output stream
int __io_putchar(int ch) {
  HAL_UART_Transmit(&STDIO_UART, (uint8_t *)&ch, 1, HAL_MAX_DELAY); 
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

    /*
     *  Initialize example peripheral
     */
    Peripheral_Init();


    // Write data to EEPROM and read int back
    uint8_t counter;
    uint8_t tx_data[2] = {COUNTER_ADDR, 0};

    while (1) {
        // set address inside EEPROM
	    HAL_I2C_IsDeviceReady(&EXAMPLE_I2C, EEPROM_ADDR, 1, HAL_MAX_DELAY);
	    HAL_I2C_Master_Transmit_IT(&EXAMPLE_I2C, EEPROM_ADDR, tx_data, 1);

	    while(!tx_completed){}
	    tx_completed = 0;

	    // read counter value
	    HAL_I2C_IsDeviceReady(&EXAMPLE_I2C, EEPROM_ADDR, 1, HAL_MAX_DELAY);
	    HAL_I2C_Master_Receive_IT(&EXAMPLE_I2C, EEPROM_ADDR, &counter, 1);

	    while(!rx_completed){}
	    rx_completed = 0;

    #ifndef STM32F042x6
	    printf("Counter value from EEPROM: %d\n", (int)counter);
    #else
        char msg[] = "Counter value from EEPROM: ";
        uint8_t eol = '\n';

        HAL_UART_Transmit(&huart2, (uint8_t*)msg, sizeof(msg)-1, HAL_MAX_DELAY);
        print_int(counter, 3);
        HAL_UART_Transmit(&huart2, &eol, 1, HAL_MAX_DELAY);
    #endif

	    // incremetn counter and write it back
	    tx_data[1] = ++counter;
	    HAL_I2C_Master_Transmit_IT(&EXAMPLE_I2C, EEPROM_ADDR, tx_data, 2);

	    while(!tx_completed){}
	    tx_completed = 0;

	    // wait 1 second and start again
	    HAL_Delay(1000);
    }
}

