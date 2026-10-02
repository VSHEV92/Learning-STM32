
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

// Main code perform two things:
//   1. Read IPSR and CONTROL register
//   2. Read SP register
//   3. Stack at busy loop
void main() {

    uint32_t val;

    // Read IPSR. Check that it's value is Zero (Thread Mode)
    val = __get_IPSR();

    // Read CONTROL. 
    // Check nPRIV bit is Zero (Privileged Level).
    // Check SPSEL bit is Zero (Main Stack Pointer).
    val = __get_CONTROL();

    // Busy Loop
    while(1) {
        __NOP();
        val++;
    }
}
