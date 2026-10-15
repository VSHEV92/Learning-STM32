#include <stm32f0xx_ll_cortex.h>
#include <stm32f0xx_ll_bus.h>
#include <stm32f0xx_ll_gpio.h>
#include <stm32f0xx_ll_rcc.h>
#include <stm32f0xx_ll_pwr.h>
#include <stm32f0xx_ll_iwdg.h>


// SysTick Handler
void SysTick_Handler() {
    static int cnt = 0;

    // Toggle GPIO LED
    // Feed watchdog
    // Increment Counter 
    LL_GPIO_TogglePin (GPIOB, LL_GPIO_PIN_3);
    LL_IWDG_ReloadCounter(IWDG);
    cnt++;


    // SysTick Handler will fire every 200ms
    // So counter reach 15 when 3 seconds elapsed
    // Now we switch to Standby mode.
    // SysTick will not fire but IWDG still runnind
    // After 2 seconds it reset MCU

    if (cnt == 15) {
        // Deep Sleep Mode in Cortex-M0
        LL_LPM_EnableDeepSleep();

        // Standby Mode in PWR register
        LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);

        __DSB();
    }
}


void IWDG_Init() {

    // Enable LSI 
    LL_RCC_LSI_Enable();
    
    // Wait for LSI is ready
    while ( !LL_RCC_LSI_IsReady() );


    // Enable IWDG 
    // Write 0xCCCC to Key Register
    LL_IWDG_Enable(IWDG);

    // Enale write access to IWDG
    // Write 0x5555 to Key Register
    LL_IWDG_EnableWriteAccess(IWDG);

    // Set Prescaled value to 256
    // LSI frequency is 40 kHz
    // IWDG inpit clock is approximatly 155 Hz
    LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_256);

    // Set Reload value to 312
    // So IWDG period is 2 second
    LL_IWDG_SetReloadCounter(IWDG, 312);

    // Poll status period to wait configuration completion 
    while ( !LL_IWDG_IsReady(IWDG) );

    // Feed watchdog
    LL_IWDG_ReloadCounter(IWDG);
}


void main() {

    // Enable GPIO Port B clock
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

    // Init strutcture to default values
    LL_GPIO_InitTypeDef gpio_init;
    LL_GPIO_StructInit(&gpio_init);

    // Configure needed values
    gpio_init.Pin = LL_GPIO_PIN_3;                     // Set LED pin to third pin
    gpio_init.Mode = LL_GPIO_MODE_OUTPUT;              // Set LED Pin as output
    gpio_init.OutputType = LL_GPIO_OUTPUT_PUSHPULL;    // Set outut type as push/pull
    gpio_init.Pull = LL_GPIO_PULL_NO;                  // Disable pullup/pulldown for LED Pin
    
    LL_GPIO_Init (GPIOB, &gpio_init);

    // IWDG Configure
    IWDG_Init();
    
    // Configure SysTick to fire every 200 ms
    SysTick_Config( SystemCoreClock/5 );
    
    // Set Sleep-On-Exit bit
    LL_LPM_EnableSleepOnExit();

    // Wait for interrupt
    __WFI();
    
}
