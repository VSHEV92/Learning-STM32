
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


void SVC_Handler() {
    // CONTROL[1], SPSEL is zero, so we use MSP
    val = __get_CONTROL();

    // Read MSP
    // Check that it is equal _end_of_stack - 32
    val = __get_MSP();

    // Read PSP
    val = __get_PSP();
}


// Main code perform following:
//   1. Fill Main Stack and check it's content 
//   2. Switch to Process Stack, fill it and check it's content
//   3. Trap to SVC handler
//   4. Infinite busy loop
void main() {

    // 1. ---------------------------------------------
    // Get MSP value and SP value
    // Find that they are same
    
    // CONTROL[1], SPSEL is zero, so we use MSP
    val = __get_CONTROL();

    // Read MSP
    // Check that it is equal _end_of_stack - 32
    val = __get_MSP();

    // Fill Stack
    // Read SP again, now it is 16 bytes lower 
    // Then read stack values
    __ASM volatile ("push {r0-r3}");

    
    // 2. ---------------------------------------------
    // Switch to Process Stack
    // Get PSP value and SP value
    // Find that they are same
    
    // Set PSP 1KB lower than _end_of_stack
    val = (uint32_t)&_end_of_stack;
    val -= 1024;
    __set_PSP(val);

    // CONTROL[1], SPSEL is one, so we use PSP
    // Need to flush pipeline because we modify CONTROL register
    __set_CONTROL(2);
    __ISB();

    // Read PSP
    // Check that it is equal (_end_of_stack - 1024)
    val = __get_PSP();

    // Fill Stack
    // Read SP again, now it is 16 bytes lower 
    // Then read stack values
    __ASM volatile ("push {r0-r3}");


    // 3. ---------------------------------------------
    // Trap to SVC handler, Stack automatically switched to MSP

    // Trap to SVC Handler
    __ASM volatile ("svc #0");

    // Busy Loop
    while(1) {}
}
