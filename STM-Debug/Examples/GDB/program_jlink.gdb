# Run OpenOCD with Nucloe F0 board configuration
# Configure OpenOCD GDB port in pipe mode (not socket mode)
# Connect to remote target using OpenOCD pipe
target extended-remote | openocd -f interface/jlink.cfg -c "transport select swd" -f target/stm32f0x.cfg -c "gdb_port pipe"

# Reset and halt target MCU
monitor reset halt

# Load executable, reset and start MCU
load
monitor reset

# Disabele GDB exit confirm 
set confirm off

# Exit from GDB
quit
