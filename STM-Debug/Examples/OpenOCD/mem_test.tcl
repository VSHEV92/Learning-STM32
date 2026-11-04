
# Get memTest function
source [find tools/memtest.tcl]

set RAM_BASE 0x20000000
set RAM_SIZE 0x1000

# Run All tests
runAllMemTests $RAM_BASE $RAM_SIZE
