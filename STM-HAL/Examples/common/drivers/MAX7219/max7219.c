#include "max7219.h"

void max7219_Init() {
    max7219_TurnOff();
    max7219_SendData(REG_SCAN_LIMIT, NUMBER_OF_DIGITS - 1);
    max7219_SendData(REG_DECODE_MODE, 0x00); // decode off
    max7219_Clean();
}

void max7219_SetIntensivity(uint8_t intensivity) {
    if (intensivity > 0x0F)
        return;

    max7219_SendData(REG_INTENSITY, intensivity);
}

void max7219_Clean() {
    for (int i = 1; i < 9; i++)
        max7219_SendData(i, MAX7219_SYM_BLANK);
}

void max7219_SendData(uint8_t addr, uint8_t data) {
    HAL_GPIO_WritePin(MAX7219_CS_GPIO_Port, MAX7219_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &addr, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(MAX7219_CS_GPIO_Port, MAX7219_CS_Pin, GPIO_PIN_SET);
}

void max7219_TurnOn(void) {
    max7219_SendData(REG_SHUTDOWN, 0x01);
}

void max7219_TurnOff(void) {
    max7219_SendData(REG_SHUTDOWN, 0x00);
}
