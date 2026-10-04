
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

// This is the first Handler, where we trap from Thread Mode
// This handler has lowet priorty. 
// Inside this handler we call SVC and jump to next hanlder
void PendSV_Handler() {
    __BKPT();

    val = __get_IPSR();
    __ASM volatile ("svc #0");
    val = __get_IPSR();
}

// This is the second handler, where we trap
// Inside it we call Unimplemented instruction to jump to HardFault
void SVC_Handler() {

    val = __get_IPSR();
    __ASM volatile ("udf #0");
    val = __get_IPSR();
}

// This is the last handler
// We check IPSR register 
// Update return adress and jump back


// Set naked attribute so handler not push anything to stack
// and we can get and change retrun address
__attribute__((naked)) void HardFault_Handler() {
    uint32_t* addr;

    val = __get_IPSR();

    // Update retrun address
    addr = (uint32_t*)__get_MSP();
    addr += 6; // offset of six words to get return address

    val = *addr;
    val += 2;
    *addr = val;

    // return from handler
    __ASM volatile ("bx lr");
}


// Main code perform following things:
//   1. Configure priorities of SVC and PendSV exceptions
//   2. Enable SVC and PendSV exceptions
//   3. Check that we are in Thread Mode
//   4. Assert PendSV to trap to Handler Mode
//   5. After return from all traps check that we again in Thread Mode
//   6. Infinite busy loop
void main() {

    // Check that we in Thread mode
    val = __get_IPSR();

    // Set PendSV priority to lower value 0b11
    NVIC_SetPriority(PendSV_IRQn, 0b11);

    // Set SVC priority to highest value 0b00
    NVIC_SetPriority(SVCall_IRQn, 0b00);

    // Assert PendSV Exception
    SCB->ICSR = (1 << SCB_ICSR_PENDSVSET_Pos);

    // Check that we in Thread mode
    val = __get_IPSR();

    // Busy Loop
    while(1) {}
}
