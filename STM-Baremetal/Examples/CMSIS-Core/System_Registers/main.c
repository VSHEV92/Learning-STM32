
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


// This is the second Handler, where we trap from Thread Mode 
// Check ICSR inside this handler
void PendSV_Handler() {
    __BKPT(0);

    // Read ICSR
    val = SCB->ICSR;

    // Clear SysTick Pending
    SCB->ICSR = SCB_ICSR_PENDSTCLR_Msk;

    // Read ICSR again
    val = SCB->ICSR;
}


// This interrupt never run.
// We disable it from PendSV Handler
void SysTick_Handler() {
    __BKPT(0);
}


// This is the second handler, where we trap
// Check ICSR inside this handler
void NMI_Handler() {
    __BKPT(0);
    val = SCB->ICSR;
}


// Main code perform following things:
//   1. Read CPUID 
//   2. Read AIRCR
//   3. Read CCR
//   4. Configure SysTick Priority
//   5. Assert exceptions using ICSR
//   6. Infinite Busy Loop
void main() {

    // Read CPUID
    val = SCB->CPUID;
    
    // Read AIRCR
    val = SCB->AIRCR;

    // Read CCR
    val = SCB->CCR;
    
    // Set SysTick priority to lowest value 0b11
    NVIC_SetPriority(SysTick_IRQn, 0b11);

    // Assert Exceptions
    val  = SCB_ICSR_NMIPENDSET_Msk;
    val |= SCB_ICSR_PENDSVSET_Msk;
    val |= SCB_ICSR_PENDSTSET_Msk;

    SCB->ICSR = val;

    // Busy Loop
    while(1) {}
}
