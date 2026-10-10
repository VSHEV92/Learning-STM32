#include <stm32f0xx.h>

#define WRITE_REG_FIELD(reg, field, val) reg = ( (reg & ~field) | ( (val << (field ## _Pos)) & field ) );

#define GPIO_OUTPUT_MODE 0b01 
#define GPIO_NO_PULLUP_DOWN 0b00


// SysTick Handler
void SysTick_Handler() {
    static int cnt = 0;

    // Toggle GPIO LED
    // Feed watchdog
    // Increment Counter 

    GPIOB->ODR ^= GPIO_ODR_3;
    IWDG->KR = 0xAAAA;
    cnt++;

    // SysTick Handler will fire every 200ms
    // So counter reach 15 when 3 seconds elapsed
    // Now we switch to Standby mode.
    // SysTick will not fire but IWDG still runnind
    // After 2 seconds it reset MCU

    if (cnt == 15) {
        // Deeo Sleep Mode in Cortex-M0
        SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

        // Standby Mode in PWR register
        PWR->CR |= PWR_CR_PDDS;

        __DSB();
    }
}

void IWDG_Init() {
    // Enable IWDG 
    IWDG->KR = 0xCCCC;

    // Enable LSI 
    RCC->CSR |= RCC_CSR_LSION;

    // Wait for LSI is ready
    while ( !(RCC->CSR & RCC_CSR_LSIRDY) ) {}

    // Enale write access to IWDG
    // Read 0x5555 to Key Register
    IWDG->KR = 0x5555;

    // Set Prescaled value to 256
    // LSI frequency is 40 kHz
    // IWDG inpit clock is approximatly 155 Hz
    IWDG->PR = 0b111;

    // Set Reload value to 312
    // So IWDG period is 2 second
    IWDG->RLR = 312; 

    // Poll status period to wait configuration completion 
    while (IWDG->SR) {}

    // Feed watchdog
    IWDG->KR = 0xAAAA;
}

void main() {
    // Enable GPIO Port B clock
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    // Set LED Pin as output
    WRITE_REG_FIELD(GPIOB->MODER, GPIO_MODER_MODER3, GPIO_OUTPUT_MODE);

    // Set LED Pin as push/pull
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT_3;

    // Disable pullup/pulldown for LED Pin 
    WRITE_REG_FIELD(GPIOB->PUPDR, GPIO_PUPDR_PUPDR3, GPIO_NO_PULLUP_DOWN);

    // IWDG Configure
    IWDG_Init();

    // Configure SysTick to fire every 200 ms
    SysTick_Config( SystemCoreClock/5 );
    
    // Set Sleep-On-Exit bit
    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;

    // Wait for interrupt
    __WFI();
    
}
