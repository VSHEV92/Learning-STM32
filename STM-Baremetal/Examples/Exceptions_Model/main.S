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
    .word  0
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
// Check current process stack.
// Inside this handler we init registers R0-R3 and R12 to some values 
// and call SVC to trap to next handler
// At the end we check return value.
.type  PendSV_Handler, %function
PendSV_Handler:  
    bkpt #0       // Set braekpoint so we stop at start of trap 

    // Check current stack values
    mrs r1, PSP
    ldr r0, [r1, #0]
    ldr r0, [r1, #4]
    ldr r0, [r1, #8]
    ldr r0, [r1, #12]
    ldr r0, [r1, #16]
    ldr r0, [r1, #20]
    ldr r0, [r1, #24]
    ldr r0, [r1, #28]

    // init registers
    ldr r0, =0xF0
    ldr r1, =0xF1
    ldr r2, =0xF2
    ldr r3, =0xF3
    ldr r4, =0xF4
    mov r12, r4

    // call SVC to jump to next handler 
    svc #0

    // read exception return value
    mov r0, lr

    bx lr



// This is the second handler, where we trap
// Check current process stack.
// Then we check return value and jump back.
.type  SVC_Handler, %function
SVC_Handler:  

    // Check current stack values
    mov r1, sp
    ldr r0, [r1, #0]
    ldr r0, [r1, #4]
    ldr r0, [r1, #8]
    ldr r0, [r1, #12]
    ldr r0, [r1, #16]
    ldr r0, [r1, #20]
    ldr r0, [r1, #24]
    ldr r0, [r1, #28]

    // read exception return value
    mov r0, lr

    bx lr





// Main code perform following things:
//   1. Configure priorities of SVC and PendSV exceptions
//   2. Enable SVC and PendSV exceptions
//   3. Switch to Process Stack Pointer
//   4. Init register to some values
//   5. Assert PendSV to trap to Handler Mode
//   6. Infinite busy loop

.section .text.main
main:

// Set PendSV priority to lower value 0b11
movs r0, #0b11
lsls r0, r0, #PendSV_Priority_Offset
ldr r1, =SHPR3
str r0, [r1]

// Set SVC priority to highest value 0b00
movs r0, #0
ldr r1, =SHPR2
str r0, [r1]

// Switch to PSP
ldr r1, =_end_of_stack
ldr r0, =1024
subs r1, r1, r0
msr PSP, r1

movs r0, #2
msr CONTROL, r0
ISB

// Init Registers
movs r0, #1
movs r1, #2
movs r2, #3
movs r3, #4
movs r4, #5
mov r12, r4

// Assert PendSV Exception
movs r4, #1
lsls r4, r4, #PendSV_Pend_Offset
ldr r5, =ICSR
str r4, [r5]


busy_loop:
    nop
    b busy_loop
    
    
