
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


# Set temporal breakpoint on GPIO toggle function
# Breakpoint will be deleted after first match
tbreak HAL_GPIO_TogglePin

# Print breakpoints info and continue
info breakpoint
continue


# Now we in HAL_GPIO_TogglePin function
# Print breakpoints info again. Temporal breakpoint deleted
info breakpoint


# Set conditional breakpoint to HAL_IncTick function
# Breakpoint will be fired only when Tick Counter (uwTick) is even
break HAL_IncTick if (uwTick % 2) == 0

# Print breakpoints info and continue
info breakpoint
continue


# Loop through conditional breakpoints 3 times
set $i = 0
while $i < 3
  printf "uwTick value is %d\n", uwTick
  continue
  set $i = $i + 1
end

# Delete breakpoints, print info and continue
delete breakpoint
info breakpoint
continue
