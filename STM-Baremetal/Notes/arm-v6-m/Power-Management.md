# Power Management

------



ARMv6-M supports the use of Wait for Interrupt (**WFI**) and Wait for Event (**WFE**) instructions as part of system power management:

- **Wait for Interrupt** provides a mechanism for hardware support of entry to one or more sleep states. Hardware can suspend execution until a wakeup event occurs.
- **Wait for Event** provides a mechanism for software to suspend program execution until a wakeup condition occurs with minimal or no impact on wakeup latency. Wait for Event provides some freedom for hardware to instigate power saving measures.

Where a processor implements power management, the System Control Register provides control and configuration of those features. The following **SCR** (System Control Register) bits control power-management functions:

- **SEVONPEND** - Configures interrupt transitions from inactive to pending state as wakeup events. This configuration means the system can use a masked interrupt as the wakeup event from WFE power-saving.
- **SLEEPONEXIT** - Enables sleep-on-exit operation, if implemented. This configuration means that, on an exception return, if no exception other than the returning exception is active, the processor suspends execution without returning from the exception.
- **SLEEPDEEP** - Selects between different levels of sleep. When this bit is set to 1, it indicates that the wakeup time from sleep state might be longer than it is when the bit set to 0. Typically, the system can use this value to determine whether it can suspend a PLL or other clock generator.

------

## Events

#### Wake Up Events

The following events are **WFE wake up events**:

- the execution of an **SEV** instruction on any other processor in a multiprocessor system.
- any exception entering the pending state if **SEVONPEND** in the System Control Register is set to 1.
- an asynchronous exception at a priority that preempts any currently active exceptions.
- a debug event with debug enabled.

#### Event Register

The Event Register is a single bit register for each processor in a multiprocessor system. When set, an Event Register indicates that an event has occurred, since the register was last cleared, that might prevent the processor having to suspend operation on issuing a WFE instruction. The following conditions apply to the Event Register:

- A reset clears the Event Register.
- Any WFE wakeup event, or the execution of an exception return instruction, sets the Event Register.
- A WFE instruction clears the Event Register. Software cannot read or write the value of the Event Register directly.

#### Send Event instruction

The Send Event instruction causes an event to be signaled to all processors in a multiprocessor system. The Send Event instruction generates a wakeup event.

The Send Event instruction is available to both unprivileged and privileged code.

#### Wait For Event instruction

The action of the Wait For Event instruction depends on the state of the Event Register:

- If the Event Register is set, the instruction clears the register and returns immediately.
- If the Event Register is clear the processor can suspend execution and enter a low-power state. It can remain in that state until the processor detects a **WFE** wakeup event or a reset.

The Wait For Event instruction, **WFE**, is available to both unprivileged and privileged code.

------

## Interrupts

The ARMv6-M architecture supports Wait For Interrupt through the hint instruction, **WFI**.

When a processor issues a WFI instruction it can suspend execution and enter a low-power state. It can remain in that state until the processor detects one of the following **WFI wake up events**:

- A reset.
- An asynchronous exception at a priority that, if **PRIMASK.PM** was set to 0, would preempt any currently active exceptions.
- If debug is enabled, a debug event.
- An **IMPLEMENTATION DEFINED** WFI wakeup event.

The processor recognizes **WFI wake up events** only after issuing the **WFI** instruction.

