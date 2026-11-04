# Enable GPIO A Clock
mww 0x40021014 0x00020014

# Set MODE Register
mww 0x48000000 0x280004a0

# Toggle ODR Register 10 time
for {set i 0} {$i < 10} {incr i} {
    mww 0x48000014 0x00
    sleep 100

    mww 0x48000014 0x20
    sleep 100
}


