# Nested Vectored Interrupt Controller

------

ARM-M provides an interrupt controller as an integral part of the exception model. The general registers associated with the **NVIC** are all accessible from a block of memory in the **SCS**.

ARM-M supports **level-sensitive** and **pulse-sensitive**, a variant of an edge sensitive, interrupt behavior. 

Pulse interrupt sources must be held long enough to be sampled reliably by the processor clock to ensure they are latched and become pending. A subsequent pulse can add the pending state to an active interrupt, making the status of the interrupt active and pending. However, multiple pulses that occur during the active period only register as a single event for interrupt scheduling.

In summary:

- Pulses held for a clock period act like edge-sensitive interrupts. These can become pending again while the interrupt is active.
- Level-sensitive interrupts become pending and activate the interrupt. The ISR then accesses the peripheral, causing it to deassert the interrupt. If the interrupt is still asserted on return from the ISR, it becomes pending again.

All **NVIC** interrupts have a programmable **priority** value and an associated **exception number**.

The **NVIC** supports the following features:

- **NVIC** interrupts can be enabled and disabled by writing to their corresponding **Interrupt Set-Enable** or **Interrupt Clear-Enable**** register bit field. When an interrupt is disabled, interrupt assertion causes the interrupt to become pending, but the interrupt does not activate. Clearing the enable bit prevents any new activation of the associated interrupt.
- Software can set or remove the pending state of **NVIC** interrupts using a complementary pair of registers, the **Set-Pending** Register and **Clear-Pending** Register.
- **NVIC** interrupts are prioritized by updating an 8-bit field within a 32-bit register, with each register supporting four interrupts.

------

### NVIC Registers



#### Interrupt Set-Enable/Clear-Enable Register, NVIC_ISER/NVIC_ICER (0xE000E100/0xE000E180)

Set and clear enable bits for each interrupt.



#### Interrupt Set-Pending/Clear-Pending Register, NVIC_ISPR/NVIC_ICPR (0xE000E200/0xE000E280)

Set and clear pending bits for each interrupt.



#### Interrupt Priority Registers, NVIC_IPR0 - NVIC_IPR7 (0xE000E400)

Sets or reads interrupt priorities.