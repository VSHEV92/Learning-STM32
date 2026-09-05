.syntax unified
.thumb


/*
 *      ISR Vector
 */
.section .isr_vector
  .word  _end_of_stack   // Main Stack Pointer (MSP)
  .word  Reset_Handler   // Reset Vector Handler




/*
 *      Main Code
 */
 
 // Define Reset_Handler lable as function. 
 // Linker set lower address bit to 1, handler
 // interwork to Thumb mode.

.section .text.isr_handlers
.global Reset_Handler
//.type  Reset_Handler, %function  
Reset_Handler:  
    b main


// Simple counter from 10 to 0
// R0 - counter register

.section .text.main
main:
    movs r0, #10

loop:
    subs r0, r0, #1
    cmp r0, #0
    bne loop

    b main
    
    
