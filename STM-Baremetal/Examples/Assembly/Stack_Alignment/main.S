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

// "a" - acllocatable flag for linker.
// Without it linker through away this section
.section .isr_vector, "a"
    .word  _end_of_stack   // Main Stack Pointer (MSP)
    .word  Reset_Handler   // Reset Vector Handler
    .word  0
    .word  0
    .word  0
    .word  0
    .word  0
    .word  0
    .word  0
    .word  0
    .word  0
    .word  SVC_Handler




/*
 *      Main Code
 */
 
 // Define ISR Vector Table
.section .text.isr_handlers

.type  Reset_Handler, %function
Reset_Handler:  
    b main

// Check Stack Address after Exception Entry
// Check xPSR alignment bit
.type  SVC_Handler, %function
SVC_Handler:  
    // Check stack pointer value
    mrs r0, MSP

    // Get xPSR from stack and check align bit
    ldr r0, [sp, #28]

    // Back to main code
    bx lr


// Main code perform following:
//   1. Read stack address and check that it is 8 bytes align
//   2. Call SVC 
//   3. Check stack address after exception retrun
//   4. Push one word to stack. Now stack is only 4 bytes align
//   5. Call SVC agin
//   6. Again check stack address after exception retrun

.section .text.main
main:
    
    // Get SP value and check that it is equal to 
    // top of RAM and 8 bytes aling
    mrs r0, MSP

    // Call SVC
    svc #0

    // Check SP value after exception return
    mrs r0, MSP

    // Push one word to stack
    push {r0}

    // Check that stack pointer is 4 bytes align only
    mrs r0, MSP

    // Call again SVC
    svc #0

    // Check SP value after exception return
    mrs r0, MSP

busy_loop:
    nop
    b busy_loop

    
    
