#include "eeprom_mem_dma.h"

#define EEPROM_ADDR 0xA0
#define BASE_ADDR 0x00
#define BUF_SIZE 30

// Set UART 2 interface as input/output stream
int __io_putchar(int ch) {
  HAL_UART_Transmit(&STDIO_UART, (uint8_t *)&ch, 1, HAL_MAX_DELAY); 
  return ch;
}

// I2C ISR Handlers
volatile uint32_t tx_completed = 0;
volatile uint32_t rx_completed = 0;

uint8_t tx_buffer[BUF_SIZE] = "Hello";
uint8_t rx_buffer[BUF_SIZE];

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c) {
  tx_completed = 1;
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
  rx_completed = 1;
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

    while (1) {
	    printf("Write to EEPROM: %s\n", tx_buffer);

	    while( HAL_I2C_IsDeviceReady(&EXAMPLE_I2C, EEPROM_ADDR, 1, HAL_MAX_DELAY) != HAL_OK ) {}

        HAL_I2C_Mem_Write_DMA(&EXAMPLE_I2C, EEPROM_ADDR, BASE_ADDR, 1, tx_buffer, strlen((char*)tx_buffer)+1);

        while(!tx_completed){}
        tx_completed = 0;

	    while( HAL_I2C_IsDeviceReady(&EXAMPLE_I2C, EEPROM_ADDR, 1, HAL_MAX_DELAY) != HAL_OK ) {}

	    HAL_I2C_Mem_Read_DMA(&EXAMPLE_I2C, EEPROM_ADDR, BASE_ADDR, 1, rx_buffer, strlen((char*)tx_buffer)+1);

	    while(!rx_completed){}
	    rx_completed = 0;

	    printf("Read from EEPROM: %s\n", rx_buffer);

	    HAL_Delay(500);
  }


}

