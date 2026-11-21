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

# -------- Disassamble -----------
# Disassamble from current PC to next 12 bytes
disassemble $pc, $pc+12

# Disassemble _write function
disassemble _write 


# Disassemble _write function with source code insertions
disassemble /m _write 


# -------- Print Sources -----------
# List 10 string from current source string
echo List Source Code 10 Lines\n
list . 

# List next 10 string from current source string
echo List Next Source Code 10 Lines\n
list

# List prevoius 10 string from current source string
echo List Previous Source Code 10 Lines\n
list -


# List _write function code
# Also print source file info
echo List _write function Source Code \n
list _write
info source 

# List main function code
# Also print source file info
echo List main function Source Code \n
list main
info source

# List 20 lines of main function
echo List 20 lines of main function Source Code \n
list main, +20


# ------------- Functions and Variables -------------
# Print all source files info
info sources

# Print all global and static variables
info variables

# Print all used HAL functions
info functions HAL_

# Print data types of some variables
ptype uwTick
ptype huart2

