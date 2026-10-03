
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
extern uint32_t _end_of_stack;

// This is the first Handler, where we trap from Thread Mode
// This handler has lowet priorty. 
// Check current process stack.
// Inside this handler call SVC to trap to next handler
// At the end we check return value.

// Set naked attribute so handler not push anything to stack
// and we can get values push to stack on exception entry
__attribute__((naked)) void PendSV_Handler() {
    __BKPT();

    // Check current stack values
    val = __get_PSP();

    // Call SVC to trap to next handler
    __ASM volatile ("svc #0");

    // Return from ecxeption
    __ASM volatile ("bx lr");
}


// This is the second handler, where we trap
// Check current process stack.
// Then we check return value and jump back.

// Set naked attribute so handler not push anything to stack
// and we can get values push to stack on exception entry
__attribute__((naked)) void SVC_Handler() {

    // Check current stack values
    val = __get_MSP();

    // Return from ecxeption
    __ASM volatile ("bx lr");
}

// Main code perform following things:
//   1. Configure priorities of SVC and PendSV exceptions
//   2. Enable SVC and PendSV exceptions
//   3. Switch to Process Stack Pointer
//   4. Assert PendSV to trap to Handler Mode
//   5. Infinite busy loop
void main() {


    // Set PendSV priority to lower value 0b11
    NVIC_SetPriority(PendSV_IRQn, 0b11);

    // Set SVC priority to highest value 0b00
    NVIC_SetPriority(SVCall_IRQn, 0b00);

    // Set PSP value 1 KB below top of stack and switch to PSP 
    val = (uint32_t)&_end_of_stack;
    val -= 1024;
    __set_PSP(val);
    __set_CONTROL(2);
    __ISB();

    // Assert PendSV Exception
    SCB->ICSR = (1 << SCB_ICSR_PENDSVSET_Pos);

    // Busy Loop
    while(1) {}
}
