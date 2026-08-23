#include "stm32f3xx.h"
/*
 * Frequency of HCLK in Hz. 
 * Used to setup Systick. 
 * Default Systick period is 1 ms.
 */
uint32_t SystemCoreClock = 8000000;

/*
 * This is encoding of HPRE feild of RCC_CFGR register.
 * Array values encode AHB prescaler value as power of two.
 * Prescaler input is HSE, HSI or PLL.
 * Prescaler output is HCLK.
 */ 
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};


/*
 * This is encoding of PPRE feild of RCC_CFGR register.
 * Array values encode APB prescaler value as power of two.
 * Prescaler input is HCLK.
 * Prescaler output is APB CLK.
 */ 
const uint8_t APBPrescTable[8]  = {0, 0, 0, 0, 1, 2, 3, 4};

/* 
 * Most earlier system init callback.
 * Called in startup.s right after stack setup.
 */
void SystemInit(void) {
    // FPU settings. Set CP10 and CP11 Full Access
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
}

/*
 * Setup timer handler that increment HAL tick counter.
 * FreeRTOS use Systick for it own perpuse so we use other timer.
 */
extern void HAL_IncTick();

#ifndef USE_FREERTOS
void SysTick_Handler(void) {
    HAL_IncTick();
}
#endif
