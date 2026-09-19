# Fault behavior

------

Under the ARMv6-M exception priority scheme, a processor handles a precise fault in one of the following ways:

- by taking a **HardFault** exception.
- by using the **Lockup** mechanism in the case of a fault arising while executing a **HardFault** or **NMI**

In ARMv6-M, faults are considered fatal. The only fault status information provided on entry to the **HardFault** handler is the **EXC_RETURN** value, that indicates whether the fault originated from Thread or Handler mode. No other exception register available for fault inspection.

The ARMv6-M  includes fault status associated with debug only, in the **DFSR** register.

The ARMv6-M and ARMv7-M architecture generally assumes that when the processor is running at priority -1 or higher, any fault or Supervisor Call that occurs is entirely **unexpected** and **fatal**. ARMv7-M requires the processor to handle most of these cases using a **Lockup** mechanism.



**Lockup** means the processor suspends normal instruction execution and enters Lockup state. When in **Lockup state**:

- the processor appears to repeatedly attempt executing instructions from the fixed address **0xFFFFFFFE**.
- the address **0xFFFFFFFE** is marked as **XN**, resulting in a further **Lockup** instruction that keeps the processor in **Lockup state**.



A processor can **exit** Lockup state in the following ways:

- If locked up at priority -1 and an **NMI** exception occurs, the **NMI** is activated as normal. The **NMI** return link is the address used for the Lockup state.
- A System **reset** occurs. This exits Lockup state and resets the system as normal.
- A halt command from a **halt** mode debug agent is issued. The processor enters Debug state with the PC set to the same value that is used for the Lockup state.



In ARMv6-M, the **AIRCR** register provides a mechanism for a **system reset**. Setting the **AIRCR.SYSRESETREQ** control bit to 1 requests a reset by an external system resource.