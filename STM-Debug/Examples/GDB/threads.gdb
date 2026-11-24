set confirm off
set pagination off

# Run OpenOCD with Nucloe F0 board configuration
# Configure OpenOCD GDB port in pipe mode (not socket mode)
# Connect to remote target using OpenOCD pipe
target extended-remote | openocd -f board/st_nucleo_f0.cfg -c "\$_TARGETNAME configure -rtos FreeRTOS" -c "gdb_port pipe"

# Reset and halt target MCU
monitor reset halt
flushregs

# Break on GPIO toggle function
# This is inside "blink_task" task
break HAL_GPIO_TogglePin
continue 

# Print all running threads
# Star show that we are in "blink_task" thead
# Seems "hello_task" is not started yet 
info threads

# Continue and print threads info again
# Now "hello_task" started and execute vTaskDelay function
continue
info threads

# Print current thread backtrace
backtrace

# Switch to thread 4 (hello_task)
# Print frame info and register
thread 4
info frame
info registers

# Switch current running thread and print registers
thread 2
info registers

# Delete breakpoints and continue execution
delete 
continue

