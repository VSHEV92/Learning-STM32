
# Run OpenOCD with Nucloe F0 board configuration
# Configure OpenOCD GDB port in pipe mode (not socket mode)
# Connect to remote target using OpenOCD pipe
target extended-remote | openocd -f board/st_nucleo_f0.cfg -c "gdb_port pipe"

# Reset and halt target MCU
monitor reset halt

# Flush GDB cache
flushregs

# Disable temporal breakpoint deletion confirmation
# Dsiable GDB paging printing
set confirm off
set pagination off


# Set write watchpoint 
watch uwTick

# Print breakpoints info and continue
info breakpoint
continue

# First write to uwTick in Reset_Handler (.data init)
# uwTick is 0 
printf "uwTick value is: %d\n", uwTick

# Continue execution
continue

# Next write to uwTick in HAL_IncTick
# uwTick is 1
# Execution stop after write
printf "uwTick value is: %d\n", uwTick

# Remove write watchpoint
# Add read watchpoint to uwTick
delete 
rwatch uwTick

# Print breakpoints info and continue
info breakpoint
continue

# Stop at uwTick read in HAL_GetTick
printf "uwTick value is: %d\n", uwTick

# Print backtrace to find functions calls
backtrace
