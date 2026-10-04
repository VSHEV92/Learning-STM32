// Use Unified Assembly Language (UAL)
// Now we can code in assambly for ARM and Thumb sets 
.syntax unified

// Next code is for Thumb instruction Set
.thumb

// Global lables
.global _end_of_stack
.global Reset_Handler
.global main


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
 
 // Define Reset_Handler lable as function. 
 // Linker set lower address bit to 1, handler
 // interwork to Thumb mode.

.section .text.isr_handlers
.type  Reset_Handler, %function
Reset_Handler:  

// Copy the data segment initializers from flash to SRAM
    ldr r0, =_start_of_data
    ldr r1, =_end_of_data
    ldr r2, =_start_of_flash_data
    movs r3, #0
    b LoopCopyDataInit

CopyDataInit:
    ldr r4, [r2, r3]
    str r4, [r0, r3]
    adds r3, r3, #4

LoopCopyDataInit:
    adds r4, r0, r3
    cmp r4, r1
    bcc CopyDataInit
  
// Zero fill the bss segment.
    ldr r2, =_start_of_bss
    ldr r4, =_end_of_bss
    movs r3, #0
    b LoopFillZerobss

FillZerobss:
    str  r3, [r2]
    adds r2, r2, #4

LoopFillZerobss:
    cmp r2, r4
    bcc FillZerobss

    b main


