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




/*
 *      Main Code
 */
 
 // Define Reset_Handler lable as function. 
 // Linker set lower address bit to 1, handler
 // interwork to Thumb mode.

.section .text.isr_handlers
.type  Reset_Handler, %function
Reset_Handler:  
    b main


// Main code perform two things:
//   1. Read IPSR and CONTROL register
//   2. Read SP register
//   2. Create an infinite downcounter
.section .text.main
main:
    // Read IPSR. Check that it's value is Zero (Thread Mode)
    mrs r0, IPSR

    // Read CONTROL. 
    // Check nPRIV bit is Zero (Privileged Level).
    // Check SPSEL bit is Zero (Main Stack Pointer).
    mrs r0, CONTROL

    // Read SP register. Check, that it set to top of RAM.
    mrs r0, msp

    // Simple counter from 10 to 0
    // R0 - counter register
cnt_init:
    movs r0, #10

cnt_loop:
    subs r0, r0, #1
    bne cnt_loop

    b cnt_init
    
    
