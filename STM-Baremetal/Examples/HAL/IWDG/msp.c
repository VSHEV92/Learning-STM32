#include <stm32f0xx.h>

IWDG_HandleTypeDef hiwdg;

void HAL_MspInit(void) {

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin   = GPIO_PIN_3,             // pin number
        .Mode  = GPIO_MODE_OUTPUT_PP,    // output push-pull mode
        .Pull  = GPIO_NOPULL,            // disable pull up/down registers
        .Speed = GPIO_SPEED_FREQ_LOW,    // set slew rate to low
    };
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    
    // Set Prescaled value to 256
    // LSI frequency is 40 kHz
    // IWDG inpit clock is approximatly 155 Hz
    // Set Reload value to 312
    // So IWDG period is 2 second
    hiwdg.Instance       = IWDG;             
    hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
    hiwdg.Init.Reload    = 312;             
    hiwdg.Init.Window    = 4095;             
    HAL_IWDG_Init(&hiwdg);

    HAL_PWR_EnableSleepOnExit();
}
