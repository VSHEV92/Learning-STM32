set confirm off
set pagination off

# Run OpenOCD with Nucloe F0 board configuration
# Configure OpenOCD GDB port in pipe mode (not socket mode)
# Connect to remote target using OpenOCD pipe
target extended-remote | openocd -f board/st_nucleo_f0.cfg -c "\$_TARGETNAME configure -rtos FreeRTOS" -c "gdb_port pipe"

# Reset and halt target MCU
monitor reset halt
flushregs

# Python package cmsis-svd was installed to HOME using command
# git clone https://github.com/bnahill/PyCortexMDebug.git ~/PyCortexMDebug
# pip install --target=$HOME/PyCortexMDebug/deps cmsis-svd

# Add PyCortexMDebug and it's dependences to PATH
python
import sys, os
sys.path.insert(0, os.path.expanduser('~/PyCortexMDebug/deps'))
sys.path.insert(0, os.path.expanduser('~/PyCortexMDebug'))
from cmdebug.svd_gdb import LoadSVD
LoadSVD()
end

# Load SVD file
svd_load STM32F0x1.svd

# Perform HAL_Init
# After that all peripheral register initialize
break HAL_Init
continue
finish

# Print all NVIC registers
svd NVIC

# Print all USART2 register in hex
svd/x USART2

# Print all GPIOA register using svd and base address
svd/x GPIOA

set $GPIOA_BASE = 0x48000000
p/x *(GPIO_TypeDef*)$GPIOA_BASE


# Toggle GPIO 10 times
set $i = 0
while $i < 10
  
  set *(unsigned int *)($GPIOA_BASE + 0x14) = (1 << 5)
  shell sleep 0.5

  set *(unsigned int *)($GPIOA_BASE + 0x14) = (0 << 5)
  shell sleep 0.5
  
  set $i = $i + 1
end


