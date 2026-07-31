#include "eeprom_mem_polling.h"

#define EEPROM_ADDR 0xA0
#define COUNTER_ADDR 0x00

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

    while (1) {

        HAL_I2C_IsDeviceReady(&EXAMPLE_I2C, EEPROM_ADDR, 1, HAL_MAX_DELAY);
	    HAL_I2C_Mem_Read(&EXAMPLE_I2C, EEPROM_ADDR, COUNTER_ADDR, 1, &counter, 1, HAL_MAX_DELAY);

	    printf("Counter value from EEPROM: %d\n", (int)counter);
        counter++;

        HAL_I2C_IsDeviceReady(&EXAMPLE_I2C, EEPROM_ADDR, 1, HAL_MAX_DELAY);
        HAL_I2C_Mem_Write(&EXAMPLE_I2C, EEPROM_ADDR, COUNTER_ADDR, 1, &counter, 1, HAL_MAX_DELAY);
	    HAL_Delay(1000);
    }
}

