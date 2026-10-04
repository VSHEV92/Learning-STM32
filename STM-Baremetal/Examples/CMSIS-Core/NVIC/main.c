
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

    IRQ0_IRQn           = 0,
    IRQ1_IRQn           = 1,
    IRQ2_IRQn           = 2,
} IRQn_Type;



// Include CMSIS Core for Cortex-M0
#include <core_cm0.h>

// Global variable to observe registers 
uint32_t val;


// IRQ0 is highest priority interrupt
// Read ICSR to check interrupt status
void IRQ0_Handler() {
    __BKPT(0);
    val = SCB->ICSR;
}


// IRQ1 is second  highest priority interrupt
// Read ICSR to check interrupt status
// Also clear pending interrupt for IRQ2
void IRQ1_Handler() {
    __BKPT(1);

    val = SCB->ICSR;
    NVIC_ClearPendingIRQ(IRQ2_IRQn);
}


// IRQ2 is lowest priority interrupt
// Never get there. It's pending bit cleared by IRQ1
void IRQ2_Handler() {
    __BKPT(2);

}


// Main code perform following things:
//   1. Configure interrupt priorities
//   2. Check enable bit behaviour. Set pending bit without enable
//   3. Check PRIMASK behaviour
//   4. Trigger all interrupts and check execution order
//   5. Infinite busy loop
void main() {


    // Set priority of handlers
    NVIC_SetPriority(IRQ0_IRQn, 0);
    NVIC_SetPriority(IRQ1_IRQn, 1);
    NVIC_SetPriority(IRQ2_IRQn, 2);
    


    // Check enable bit behaviour. Set pending bit without enable
    // Set IRQ0 pending bit without enable
    NVIC_SetPendingIRQ(IRQ0_IRQn);
    __DSB();

    // Now set enable bit to trigger interrupt
    __BKPT(3);
    NVIC_EnableIRQ(IRQ0_IRQn);



    // Check PRIMASK behaviour. Disable interrupts
    __disable_irq();
    NVIC_SetPendingIRQ(IRQ0_IRQn);

    // Now enable interrupts
    __BKPT(4);
    __enable_irq();



    // Trigger all three interrupts and check order
    NVIC_DisableIRQ(IRQ0_IRQn);
    
    NVIC_SetPendingIRQ(IRQ0_IRQn);
    NVIC_SetPendingIRQ(IRQ1_IRQn);
    NVIC_SetPendingIRQ(IRQ2_IRQn);

    NVIC->ISER[0] = 0b111;
    __DSB();


    __BKPT(5);
    // Busy Loop
    while(1) {}
}
