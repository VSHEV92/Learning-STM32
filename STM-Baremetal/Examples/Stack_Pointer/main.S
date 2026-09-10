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


.type  SVC_Handler, %function
SVC_Handler:  

    // CONTROL[1], SPSEL is zero, so we use MSP
    mrs r0, CONTROL

    // Read MSP and SP
    // Check that they are same and equal _end_of_stack
    mrs r0, MSP
    add r0, sp, #0 
    
    // Data pushed to PSP, find it's top
    mrs r1, PSP

    // Read all data pushed to stack
    ldr r0, [r1, #0]    // R0
    ldr r0, [r1, #4]    // R1
    ldr r0, [r1, #8]    // R2
    ldr r0, [r1, #12]   // R3
    ldr r0, [r1, #16]   // R12
    ldr r0, [r1, #20]   // R14 (LR)
    ldr r0, [r1, #24]   // Return Address
    ldr r0, [r1, #28]   // xPSR

    // Back to main code
    bx lr


// Main code perform following:
//   1. Fill Main Stack and check it's content 
//   2. Switch to Process Stack, fill it and check it's content
//   3. Trap to SVC handler
//   4. Infinite busy loop
.section .text.main
main:
    
    // 1. ---------------------------------------------
    // Get MSP value and SP value
    // Find that they are same
    
    // CONTROL[1], SPSEL is zero, so we use MSP
    mrs r0, CONTROL

    // Read MSP and SP
    // Check that they are same and equal _end_of_stack
    mrs r0, MSP
    add r0, sp, #0 

    // Fill Stack
    movs r0, #0
    movs r1, #1
    movs r2, #2
    movs r3, #3
    push {r0-r3}

    // Read SP again, now it is 16 bytes lower 
    add r0, sp, #0 
    
    // Read stack values
    ldr r0, [sp, #0]
    ldr r0, [sp, #4]
    ldr r0, [sp, #8]
    ldr r0, [sp, #12]
    
    // 2. ---------------------------------------------
    // Switch to Process Stack
    // Get PSP value and SP value
    // Find that they are same
    
    // Set PSP 1KB lower than _end_of_stack
    ldr r0, =_end_of_stack
    ldr r1, =1024
    subs r0, r1
    msr PSP, r0

    // CONTROL[1], SPSEL is one, so we use PSP
    movs r0, #2
    msr CONTROL, r0
    
    // Need to flush pipeline because we modify CONTROL register
    ISB   

    // Check new CONTROL.SPSEL value
    mrs r0, CONTROL

    // Read PSP and SP
    // Check that they are same and equal (_end_of_stack - 1024)
    mrs r0, PSP
    add r0, sp, #0 

    // Fill Stack
    ldr r0, =0xF0
    ldr r1, =0xF1
    ldr r2, =0xF2
    ldr r3, =0xF3
    push {r0-r3}

    // Read SP again, now it is 16 bytes lower 
    add r0, sp, #0 
    
    // Read stack values
    ldr r0, [sp, #0]
    ldr r0, [sp, #4]
    ldr r0, [sp, #8]
    ldr r0, [sp, #12]

    // Restore R0 value
    ldr r0, =0xF0

    // 3. ---------------------------------------------
    // Trap to SVC handler, Stack automatically switched to MSP
    // Get MSP value and SP value
    // Get all values popped to stack on handler entry

    // Trap to SVC Handler
    svc #0

    // Read process stack values again
    ldr r0, [sp, #0]
    ldr r0, [sp, #4]
    ldr r0, [sp, #8]
    ldr r0, [sp, #12]

busy_loop:
    nop
    b busy_loop

    
    
