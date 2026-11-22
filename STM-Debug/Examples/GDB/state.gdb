set confirm off
set pagination off

# Run OpenOCD with Nucloe F0 board configuration
# Configure OpenOCD GDB port in pipe mode (not socket mode)
# Connect to remote target using OpenOCD pipe
target extended-remote | openocd -f board/st_nucleo_f0.cfg -c "gdb_port pipe"

# Reset and halt target MCU
monitor reset halt
flushregs


# Set Breakpoint to SysTick_Handler function
# Set breakpoint before data pushed to stack

set $ba = &SysTick_Handler
break *$ba
continue

# Get CPU state (run, halt, and so on)
monitor targets

# Print all register
info registers

# Print PC counter value in hex
print/x $pc

# Print current instruction
x/i $pc

# Print data stacked on function 
# Explore 8 words in hex
x/8wx $sp

# Show backtrace. Compare Return Address in Stack with value from trace
backtrace

# Print SCB registers
set $SCB_BASE = 0xE000ED00
p/x *(SCB_Type *)$SCB_BASE

# Print NVIC registers
set $NVIC_BASE = 0xE000E100
p/x *(NVIC_Type *)$NVIC_BASE

# Print SysTick registers
set $SysTick_BASE = 0xE000E010
p/x *(SysTick_Type *)$SysTick_BASE
p (*(SysTick_Type *)$SysTick_BASE).LOAD
