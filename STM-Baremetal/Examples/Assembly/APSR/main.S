// Use Unified Assembly Language (UAL)
// Now we can code in assambly for ARM and Thumb sets 
.syntax unified

// Next code is for Thumb instruction Set
.thumb

// Global lables
.global _end_of_stack
.global Reset_Handler


/*
 *      ISR Vector
 */
.section .isr_vector, "a"
    .word  _end_of_stack   // Main Stack Pointer (MSP)
    .word  Reset_Handler   // Reset Vector Handler




/*
 *      Main Code
 */
 

// Reset Handler Just jump to main
.section .text.isr_handlers
.type  Reset_Handler, %function
Reset_Handler:  
    b main


// Main code perform following things:
//   1. Set N bit in APSR
//   2. Set Z bit in APSR
//   3. Set C bit in APSR
//   4. Set V bit in APSR
//   5. Ends with dummy busy loop
.section .text.main
main:

// Set N - Negative flag. 
// This bit is when result of operation is negative.
// This is when MSB is one.
movs r0, #0
adds r0, #-1
mrs r0, APSR

// Set Z - Negative flag. 
// This bit is when result of operation is zero value.
// This is when bits are zeros.
movs r0, #0
mrs r0, APSR

// Set C - Carry flag. 
// This bit is when result of arithmetic operations has carry or borrow bit.
// Or when bit with value of one shifted from register.
movs r0, #3
lsrs r0, #1
mrs r0, APSR

// Set V - OVerflow flag. 
// This bit is when result of signed arithmetic operations has overflow.
// This happens when result of operation has sign different from sign of operands.

// Set most positive value (MSB is zero, other bits are ones) to r0
movs r0, #1
lsls r0, #31
mvns r0, r0

// Add this value with one to get overflow
adds r0, #1
mrs r0, APSR


busy_loop:
    nop
    b busy_loop
    
    
