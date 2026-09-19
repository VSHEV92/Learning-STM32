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
    .word  _end_of_stack
    .word  Reset_Handler
    .word  0
    .word  HardFault_Handler


/*
 *      Main Code
 */
 
 
 // Define ISR Vector Table
.section .text.isr_handlers

// From Reset Handler we only jump to main 
.type  Reset_Handler, %function
Reset_Handler:  
    b main


// HardFault Handler
// Read IPSR to check that we in HandFault hander
// Call UDF (undefined) instruction to cuase Lockup
.type  HardFault_Handler, %function
HardFault_Handler:  
    mrs r0, IPSR
    udf #0




// Main code UDF (undefined) instruction to 
// jump to HaedFault handler

.section .text.main
main:
    udf #0
