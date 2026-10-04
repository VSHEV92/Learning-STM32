
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


// HardFault Handler
// Read IPSR to check that we in HandFault hander
// Call UDF (undefined) instruction to cuase Lockup
void HardFault_Handler() {
    uint32_t val = __get_IPSR();
    __ASM volatile ("udf #0");

    // To suppress warning
    (void)val;
}


// Main code UDF (undefined) instruction to 
// jump to HaedFault handler
void main() {
    __ASM volatile ("udf #0");
}
