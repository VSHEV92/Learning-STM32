#include "max7219_blink.h"

// SPI Handle
SPI_HandleTypeDef hspi1;

void Peripheral_Init() {

    // Enable clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_SPI1_CLK_ENABLE();


    // Initialize GPIO pins
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // CS Pin
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin = MAX7219_CS_Pin;
    HAL_GPIO_Init(MAX7219_CS_GPIO_Port, &GPIO_InitStruct);

    // SPI Pins
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pin       = GPIO_PIN_5 | GPIO_PIN_7;
    
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    

    // Initialize SPI
    hspi1.Instance               = SPI1;                     // SPI instance
    hspi1.Init.Mode              = SPI_MODE_MASTER;          // enable master mode
    hspi1.Init.Direction         = SPI_DIRECTION_2LINES;     // full duplex mode
    hspi1.Init.NSS               = SPI_NSS_SOFT;             // Manage SS pin by software
    hspi1.Init.DataSize          = SPI_DATASIZE_8BIT;        // 8 bits per transfer
    hspi1.Init.CLKPolarity       = SPI_POLARITY_LOW;         // CPOL value
    hspi1.Init.CLKPhase          = SPI_PHASE_1EDGE;          // CPHA value
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16; // SCL clock prescaler
    hspi1.Init.FirstBit          = SPI_FIRSTBIT_MSB;         // send MSB bit first
    
    HAL_SPI_Init(&hspi1);


    // Initialize MAX7219 driver 
    max7219_Init();

}
