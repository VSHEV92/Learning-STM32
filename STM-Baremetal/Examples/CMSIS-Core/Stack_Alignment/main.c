
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


// Check Stack Address after Exception Entry
// Check xPSR alignment bit
__attribute__((naked)) void SVC_Handler() {

    // Read MSP
    val = __get_MSP();

    // Read xPSR from stack
    val = *((uint32_t*)val + 7);

    // return from handler
    __ASM volatile ("bx lr");
}


// Main code perform following:
//   1. Read stack address and check that it is 8 bytes align
//   2. Call SVC 
//   3. Check stack address after exception retrun
//   4. Push one word to stack. Now stack is only 4 bytes align
//   5. Call SVC agin
//   6. Again check stack address after exception retrun
void main() {

    // Get SP value and check that it is 8 bytes aling
    val = __get_MSP();

    // Call SVC
    __ASM volatile ("svc #0");

    // Check SP value after exception return
    val = __get_MSP();
    
    // Push one word to stack
    __ASM volatile ("push {r0}");

    // Check that stack pointer is 4 bytes align only
    val = __get_MSP();

    // Call again SVC
    __ASM volatile ("svc #0");

    // Check SP value after exception return
    val = __get_MSP();

    // Busy Loop
    while(1) {}
}
