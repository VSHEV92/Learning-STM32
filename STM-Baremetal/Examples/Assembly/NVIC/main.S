// Use Unified Assembly Language (UAL)
// Now we can code in assambly for ARM and Thumb sets 
.syntax unified

// Next code is for Thumb instruction Set
.thumb

// Global lables
.global _end_of_stack
.global Reset_Handler


.equ ICSR, 0xE000ED04

.equ NVIC_ISER, 0xE000E100
.equ NVIC_ICER, 0xE000E180
.equ NVIC_ISPR, 0xE000E200
.equ NVIC_ICPR, 0xE000E280

.equ NVIC_IPR0, 0xE000E400

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
    .word  0
    .word  0
    .word  0
    .word  0
    .word  0
    .word  IRQ0_Handler
    .word  IRQ1_Handler
    .word  IRQ2_Handler




/*
 *      Main Code
 */
 
 
 // Define ISR Vector Table
.section .text.isr_handlers

// From Reset Handler we only jump to main 
.type  Reset_Handler, %function
Reset_Handler:  
    b main


// IRQ0 is highest priority interrupt
// Read ICSR to check interrupt status
.type  IRQ0_Handler, %function
IRQ0_Handler:  
    bkpt #0

    ldr r1, =ICSR
    ldr r0, [r1]

    bx lr


// IRQ1 is second  highest priority interrupt
// Read ICSR to check interrupt status
// Also clear pending interrupt for IRQ2
.type  IRQ1_Handler, %function
IRQ1_Handler:  
    bkpt #1

    ldr r1, =ICSR
    ldr r0, [r1]

    movs r0, #0b100
    ldr r1, =NVIC_ICPR
    str r0, [r1]

    bx lr


// IRQ2 is lowest priority interrupt
// Never get there. It's pending bit cleared by IRQ1
.type  IRQ2_Handler, %function
IRQ2_Handler:  
    bkpt #2
    bx lr





// Main code perform following things:
//   1. Configure interrupt priorities
//   2. Check enable bit behaviour. Set pending bit without enable
//   3. Check PRIMASK behaviour
//   4. Trigger all interrupts and check execution order
//   5. Infinite busy loop

.section .text.main
main:

// Configure interrupt priorities
// IRQ0 - 0
// IRQ0 - 1
// IRQ0 - 2

movs r0, #2
lsls r0, r0, #8

adds r0, r0, #1 
lsls r0, r0, #8

ldr r1, =NVIC_IPR0
str r0, [r1]


// Check enable bit behaviour. Set pending bit without enable
// Set IRQ0 pending bit without enable
movs r0, #1
ldr r1, =NVIC_ISPR
str r0, [r1]
DSB

// Now set enable bit to trigger interrupt
bkpt #3
ldr r1, =NVIC_ISER
str r0, [r1]
WFI


// Check PRIMASK behaviour
CPSID i

movs r0, #1
ldr r1, =NVIC_ISPR
str r0, [r1]

// Now enable interrupts
bkpt #4
CPSIE i
WFI


// Trigger all three interrupts and check order
movs r0, #0b111

ldr r1, =NVIC_ISER
str r0, [r1]

ldr r1, =NVIC_ISPR
str r0, [r1]
WFI


bkpt #5

busy_loop:
    nop
    b busy_loop
    
    
