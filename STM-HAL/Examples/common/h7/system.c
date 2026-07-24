#include "stm32h7xx.h"

uint32_t SystemCoreClock = 64000000;
uint32_t SystemD2Clock = 64000000;
const uint8_t D1CorePrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

/* 
 * Most earlier system init callback.
 * Called in startup.s right after stack setup.
 */
void SystemInit(void) {
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
    SCB->SCR |= SCB_SCR_SEVONPEND_Msk;
}

/* 
 * Setup power regulator.
 * Called in startup.s right after stack setup.
 */
void ExitRun0Mode(void) {
    PWR->CR3 &= ~(PWR_CR3_LDOEN);
    while ((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == 0U) {}
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
