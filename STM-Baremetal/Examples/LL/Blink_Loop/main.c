#include <stm32f0xx_ll_bus.h>
#include <stm32f0xx_ll_gpio.h>
#include <stm32f0xx_ll_utils.h>

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
    
    // Init SysTick to 1 ms period
    LL_Init1msTick(SystemCoreClock);
    
    // Toggle LED with 500 ms delay
    while(1) {
        LL_GPIO_TogglePin (GPIOB, LL_GPIO_PIN_3);
        LL_mDelay(500);
    }
    
}
