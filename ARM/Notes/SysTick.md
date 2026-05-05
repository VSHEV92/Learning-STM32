# ARM System Timer

------

An Armv7-M implementation must include a system timer, SysTick, that provides a simple, 24-bit clear-on-write, decrementing, wrap-on-zero counter with a flexible control mechanism.

Timer Registers:

- **SysTick Control and Status Register** (**SYST_CSR**, 0xE000E010):

  -  configures the SysTick clock source (**CLKSOURCE**), 
  - enables the counter (**ENABLE**), 
  - enables the SysTick interrupt (**TICKINT**), 
  - indicates the counter status (**COUNTFLAG**). When the counter transitions to zero, it sets the COUNTFLAG status bit to 1. Reading the COUNTFLAG status bit clears it to 0.

  

- **SysTick Reload Value Register** (**SYST_RVR**, 0xE000E014) - A counter reload value register. This provides the wrap value for the counter. Writing a value of zero to SYST_RVR disables the counter on the next wrap. The SysTick counter logic maintains this counter value of zero after the wrap.

  

- **SysTick Current Value Register** (**SYST_CVR**, 0xE000E018) - A counter current value register. When the counter reaches zero, it reloads the value in SYST_RVR on the next clock edge, Any write to the register clears the register to zero. This causes the SysTick logic to reload SYST_CVR from SYST_RVR on the next timer clock

  .

- **SysTick Calibration value Register** (**SYST_CALIB**, 0xE000E01C) - A calibration value register. This indicates the preload value required for a 10ms (100Hz) system clock.



When the processor is halted in **Debug** state, the counter does not decrement.

The timer is clocked by a reference clock. Whether the reference clock is the processor clock or an external clock source is implementation defined.