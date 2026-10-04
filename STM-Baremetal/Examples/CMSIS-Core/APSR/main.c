
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

// Main code perform following things:
//   1. Set N bit in APSR
//   2. Set Z bit in APSR
//   3. Set C bit in APSR
//   4. Set V bit in APSR
//   5. Ends with dummy busy loop
void main() {

    uint32_t val, udat;
    int32_t dat;

    // Set N - Negative flag. 
    // This bit is when result of operation is negative.
    // This is when MSB is one.
    dat = -1;
    val = __get_APSR();

    // Set Z - Negative flag. 
    // This bit is when result of operation is zero value.
    // This is when bits are zeros.
    dat = 0;
    val = __get_APSR();
    
    // Set C - Carry flag. 
    // This bit is when result of arithmetic operations has carry or borrow bit.
    // Or when bit with value of one shifted from register.
    udat = ~0;
    udat >>= 1;
    val = __get_APSR();
    
    // Set V - OVerflow flag. 
    // This bit is when result of signed arithmetic operations has overflow.
    // This happens when result of operation has sign different from sign of operands.
    dat = 1 << 31;
    dat = ~dat;
    dat++;
    val = __get_APSR();

    // Busy Loop
    (void)val;
    while(1) {}
}
