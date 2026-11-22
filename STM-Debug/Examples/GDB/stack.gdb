set confirm off
set pagination off

# Run OpenOCD with Nucloe F0 board configuration
# Configure OpenOCD GDB port in pipe mode (not socket mode)
# Connect to remote target using OpenOCD pipe
target extended-remote | openocd -f board/st_nucleo_f0.cfg -c "gdb_port pipe"

# Reset and halt target MCU
monitor reset halt
flushregs


# Set Breakpoint to HAL_UART_Transmit function
break HAL_UART_Transmit
continue

# Show backtrace. 
# We can see all function calls
backtrace

# Print current stack frame info
info frame

# Print current frame function arguments
info args

# Print current frame local variables
info locals

# Switch to one frame UP (_write) and print args
up
info args

# Switch to frame 12 (HAL_TIM_IRQHandler) and print args
# Also print input arg type and value as struct
frame 12
info args
ptype htim
p *(TIM_HandleTypeDef*)htim
p htim.Init

# Switch back to top frame
frame 0




