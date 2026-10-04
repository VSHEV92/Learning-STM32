
// Usually this comes from CMSIS Device

// Number of priority bits
#define __NVIC_PRIO_BITS 2

// Exception numbers table
typedef enum
{
    NonMaskableInt_IRQn = -14,    
    HardFault_IRQn      = -13,    
    SVCall_IRQn         = -5,     
    PendSV_IRQn         = -2,     
    SysTick_IRQn        = -1,     
} IRQn_Type;



// Include CMSIS Core for Cortex-M0
#include <core_cm0.h>

// Global variable to observe registers 
uint32_t val;

// CPU Frequency
#define CPU_FREQ_HZ 8000000


// Stop on enrty to this hander.
// Read SysTick Counter Flag.
// Just to show that SysTick fires every second.
void SysTick_Handler() {
    __BKPT(0);
    val = SysTick->CTRL;
    val = SysTick->CTRL;
}




// Main code perform following things:
//   1. Configure to go to Sleep on Exception Entry
//   2. Configure SysTick Reload Value
//   3. Reset SysTick Current value and flag
//   4. Enable SysTick
//   5. Wait for interrupts
void main() {

    // Configure Sleep on Exception Return mode
    SCB->SCR = SCB_SCR_SLEEPONEXIT_Msk;
    
    // Configure Reload value.
    // Defalut CPU frequency is 8 MHz
    // Need to set this value minus one
    // This cause fire ecxeptions every second
    SysTick->LOAD = CPU_FREQ_HZ - 1;


    // Store any value to Current Value Regiser
    // This clear this register and also clear Counter Flag
    // Check Current value at the end
    SysTick->VAL = 0;


    // Set SysTick clock to Processor clock (bit[2] == 1)
    // Enable SysTick interrupts (bit[1] == 1)
    // Enable SysTick (bit[0] == 1)
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                     SysTick_CTRL_TICKINT_Msk   |
                     SysTick_CTRL_ENABLE_Msk;

    // Wait for interrupts
    __DSB();
    __WFI();
}

