// Use Unified Assembly Language (UAL)
// Now we can code in assambly for ARM and Thumb sets 
.syntax unified

// Next code is for Thumb instruction Set
.thumb

// Global lables
.global _end_of_stack
.global Reset_Handler

.equ SCR, 0xE000ED10

.equ SYST_CSR, 0xE000E010 
.equ SYST_RVR, 0xE000E014 
.equ SYST_CVR, 0xE000E018 

.equ CPU_FREQ_HZ, 8000000

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



// Stop on enrty to this hander.
// Read SysTick Counter Flag.
// Just to show that SysTick fires every second.
.type  SysTick_Handler, %function
SysTick_Handler:  
    bkpt #0

    ldr r1, =SYST_CSR
    ldr r0, [r1]
    ldr r0, [r1]

    bx lr


// Main code perform following things:
//   1. Configure to go to Sleep on Exception Entry
//   2. Configure SysTick Reload Value
//   3. Reset SysTick Current value and flag
//   4. Enable SysTick
//   5. Wait for interrupts

.section .text.main
main:

// Configure Sleep on Exception Return mode
ldr r1, =SCR
movs r0, #0b10
str r0, [r1]


// Configure Reload value.
// Defalut CPU frequency is 8 MHz
// Need to set this value minus one
// This cause fire ecxeptions every second
ldr r0, =CPU_FREQ_HZ
subs r0, r0, #1
ldr r1, =SYST_RVR
str r0, [r1]

// Store any value to Current Value Regiser
// This clear this register and also clear Counter Flag
// Check Current value at the end
ldr r1, =SYST_CVR
str r0, [r1]
ldr r0, [r1]

// Set SysTick clock to Processor clock (bit[2] == 1)
// Enable SysTick interrupts (bit[1] == 1)
// Enable SysTick (bit[0] == 1)
movs r0, #0b111
ldr r1, =SYST_CSR
str r0, [r1]

// Wait for interrupts
DSB
wfi
    
    
