// Use Unified Assembly Language (UAL)
// Now we can code in assambly for ARM and Thumb sets 
.syntax unified

// Next code is for Thumb instruction Set
.thumb

// Global lables
.global _end_of_stack
.global Reset_Handler

// Corntex-M0 System Registers
.equ ICSR,  0xE000ED04 
.equ SHPR2, 0xE000ED1C
.equ SHPR3, 0xE000ED20

.equ PendSV_Pend_Offset,     28 
.equ PendSV_Priority_Offset, 22 

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
    .word  0
    .word  0
    .word  0
    .word  0
    .word  0
    .word  0
    .word  0
    .word  SVC_Handler
    .word  0
    .word  0
    .word  PendSV_Handler




/*
 *      Main Code
 */
 
 
 // Define ISR Vector Table
.section .text.isr_handlers

// From Reset Handler we only jump to main 
.type  Reset_Handler, %function
Reset_Handler:  
    b main


// This is the first Handler, where we trap from Thread Mode
// This handler has lowet priorty. 
// Inside this handler we call SVC and jump to next hanlder
.type  PendSV_Handler, %function
PendSV_Handler:  
    bkpt #0       // Set braekpoint so we stop at start of trap 
    mrs r0, IPSR
    svc #0
    mrs r0, IPSR
    bx lr


// This is the second handler, where we trap
// Inside it we call Unimplemented instruction to jump to HardFault
.type  SVC_Handler, %function
SVC_Handler:  
    mrs r0, IPSR
    udf #0
    mrs r0, IPSR
    bx lr


// This is the last handler
// We check IPSR register 
// Update return adress and jump back
.type  HardFault_Handler, %function
HardFault_Handler:  
    mrs r0, IPSR
    mrs r1, MSP
    adds r1, #0x18
    ldr r0, [r1]
    adds r0, #2
    str r0, [r1]
    bx lr



// Main code perform following things:
//   1. Configure priorities of SVC and PendSV exceptions
//   2. Enable SVC and PendSV exceptions
//   3. Check that we are in Thread Mode
//   4. Assert PendSV to trap to Handler Mode
//   5. After return from all traps check that we again in Thread Mode
//   6. Infinite busy loop

.section .text.main
main:

// Check that we in Thread mode
mrs r0, IPSR

// Set PendSV priority to lower value 0b11
movs r0, #0b11
lsls r0, r0, #PendSV_Priority_Offset
ldr r1, =SHPR3
str r0, [r1]

// Set SVC priority to highest value 0b00
movs r0, #0
ldr r1, =SHPR2
str r0, [r1]

// Assert PendSV Exception
movs r0, #1
lsls r0, r0, #PendSV_Pend_Offset
ldr r1, =ICSR
str r0, [r1]


// Check that we again in Thread mode
busy_loop:
    mrs r0, IPSR
    b busy_loop
    
    
