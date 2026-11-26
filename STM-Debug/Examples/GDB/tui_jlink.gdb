
# Run OpenOCD with Nucloe F0 board configuration
# Configure OpenOCD GDB port in pipe mode (not socket mode)
# Connect to remote target using OpenOCD pipe
target extended-remote | openocd -f interface/jlink.cfg -c "transport select swd" -f target/stm32f0x.cfg -c "gdb_port pipe"

# Reset and halt target MCU
monitor reset halt

# Flush GDB cache
flushregs

# Print PC and SP register
printf "Program counter is: 0x%x\n", $pc
printf "Stack pointer is: 0x%x\n", $sp

# Set breakpoint to main function
break main
continue

# Now MCU is halted on main function
# Enable TUI
tui enable

# Switch to Assambler and Register layout
layout asm
layout regs

# Focus on command line
focus cmd
