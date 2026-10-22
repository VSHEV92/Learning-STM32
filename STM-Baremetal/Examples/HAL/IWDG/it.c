#include <stm32f0xx.h>
#include <stm32f0xx_ll_cortex.h>
#include <stm32f0xx_ll_pwr.h>

#define TOGGLE_DELAY_MS 300 

extern IWDG_HandleTypeDef hiwdg;

void SysTick_Handler(void) {
    static int delay_cnt = 0;
    static int toggle_cnt = 0;
    
    HAL_IncTick();
    
    delay_cnt++;
    if (delay_cnt == TOGGLE_DELAY_MS) {
        
        // Toggle GPIO LED
        // Feed watchdog
        // Increment Toggle Counter 
	    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
        HAL_IWDG_Refresh(&hiwdg);
        toggle_cnt++;
        delay_cnt = 0;

        // SysTick Handler will fire every 200ms
        // So counter reach 15 when 3 seconds elapsed
        // Now we switch to Standby mode.
        // SysTick will not fire but IWDG still runnind
        // After 2 seconds it reset MCU

        if (toggle_cnt == 15) {
            // Deep Sleep Mode in Cortex-M0
            LL_LPM_EnableDeepSleep();

            // Standby Mode in PWR register
            LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);

            __DSB();
        }
    }

}

