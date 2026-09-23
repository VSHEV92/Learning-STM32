// Use Unified Assembly Language (UAL)
// Now we can code in assambly for ARM and Thumb sets 
.syntax unified

// Next code is for Thumb instruction Set
.thumb

// Global lables
.global _end_of_stack
.global Reset_Handler

.equ NMI_Pend_Set_Offset,     31 
.equ PendSV_Pend_Set_Offset,  28 
.equ SysTick_Pend_Set_Offset, 26

.equ SysTick_Pend_Clr_Offset, 25 

.equ SysTick_Priority_Offset, 30 

.equ CPU_Reset_Value, 0x05FA0004

// Corntex-M0 System Registers
.equ CPUID, 0xE000ED00 
.equ ICSR,  0xE000ED04 
.equ VTOR,  0xE000ED08 
.equ AIRCR, 0xE000ED0C 
.equ SCR,   0xE000ED10 
.equ CCR,   0xE000ED14 
.equ SHPR2, 0xE000ED1C
.equ SHPR3, 0xE000ED20

/*
 *      ISR Vector
 */

// "a" - acllocatable flag for linker.
// Without it linker through away this section
.section .isr_vector, "a"
    .word  _end_of_stack
    .word  Reset_Handler
    .word  NMI_Handler
    .word  0
    .word  0
    .word  0
    .word  0
    .word  0
    .word  0
    .word  0
    .word  0
    .word  0
    .word  0
    .word  0
    .word  PendSV_Handler
    .word  SysTick_Handler




/*
 *      Main Code
 */
 
 
 // Define ISR Vector Table
.section .text.isr_handlers

// From Reset Handler we only jump to main 
.type  Reset_Handler, %function
Reset_Handler:  
    b main


// This is the second Handler, where we trap from Thread Mode 
// Check ICSR inside this handler
.type  PendSV_Handler, %function
PendSV_Handler:  
    bkpt #0

    // Read ICSR 
    ldr r1, =ICSR
    ldr r0, [r1]
    
    // Clear SysTick Pending
    movs r0, #1
    lsls r0, r0, #SysTick_Pend_Clr_Offset
    str r0, [r1]

    // Read ICSR again 
    ldr r1, =ICSR
    ldr r0, [r1]

    // Return
    bx lr

// This interrupt never run.
// We disable it from PendSV Handler
.type  SysTick_Handler, %function
SysTick_Handler:  
    bkpt #0
    bx lr


// This is the second handler, where we trap
// Check ICSR inside this handler
.type  NMI_Handler, %function
NMI_Handler:  
    // Read ICSR 
    ldr r1, =ICSR
    ldr r0, [r1]

    // Return
    bx lr





// Main code perform following things:
//   1. Read CPUID 
//   2. Read VTOR
//   3. Read AIRCR
//   4. Read CCR
//   5. Configure SysTick Priority
//   6. Assert exceptions using ICSR
//   7. Infinite Busy Loop

.section .text.main
main:

// Read CPUID
ldr r1, =CPUID
ldr r0, [r1]

// Read VTOR
ldr r1, =VTOR
ldr r0, [r1]

// Read AIRCR
ldr r1, =AIRCR
ldr r0, [r1]

// Read CCR
ldr r1, =CCR
ldr r0, [r1]

// Set SysTick priority to lowest value 0b11
movs r0, #3
lsls r0, r0, #SysTick_Priority_Offset
ldr r1, =SHPR3
str r0, [r1]

// Assert Exceptions
movs r0, #0

movs r2, #1
lsls r2, r2, #NMI_Pend_Set_Offset
orrs r0, r0, r2

movs r2, #1
lsls r2, r2, #PendSV_Pend_Set_Offset
orrs r0, r0, r2

movs r2, #1
lsls r2, r2, #SysTick_Pend_Set_Offset
orrs r0, r0, r2

ldr r1, =ICSR
str r0, [r1]

// Wait for Exception
DSB

busy_loop:
    nop
    b busy_loop
    
    
