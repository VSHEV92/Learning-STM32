# CoreSight Specification

------

### Structure of the CoreSight architecture

The CoreSight architecture provides a system-wide solution for real-time debug and collecting trace information.

The CoreSight architecture can be used to design CoreSight components, that can be combined with other CoreSight components and processors that comply with the CoreSight architecture to make up a CoreSight system.

The CoreSight architecture comprises 3 levels:

#### Visible component architecture

The CoreSight visible component architecture specifies what a component must implement to comply with the CoreSight architecture. The visible component architecture is visible to the programming interface and to tools that access the device. All CoreSight components must comply with the visible component architecture.

#### Reusable component architecture

The CoreSight reusable component architecture specifies the rules that the implementation of the physical interface of a CoreSight component must follow to work correctly with other reusable CoreSight components.

#### System architecture

The following CoreSight architectural requirements ensure seamless integration of elements that comply with the CoreSight architecture

------

### About the Visible Component Architecture

The visible component architecture specifies aspects of components that are visible to the programming interface and to tools that access the device.

The basic register structure is based on the Peripheral ID Register structure for Arm CoreLink components, and comprises a set of word-aligned 32-bit registers that can be divided into the following categories:

- Component and Peripheral Identification Registers 
- Component-specific control registers

This register structure must be supported by every component that implements a CoreSight compliant programmers’ model. Specification describes many registers to identify and use components.

Last registers of component address space is **CIDR0-CIDR3** and **PIDR0-PIDR7**.



Components that have the value **0x9** assigned to the **CIDR1.CLASS** field in the Component Identification Register are CoreSight components. CoreSight components must implement an extra set of registers, referred to as the CoreSight management registers. Addresses 0xF00 to 0xFCC are reserved for use by CoreSight management registers.



CoreSight system components can have various interface types. A component specifies which interfaces are present, and whether they act as Transmitter or Receiver. Each interface type defines a set of control signals that enable a debugger to determine which other components are connected to it. These signals are referred to as topology detection signals. During **topology detection**, a debugger probes each interface to determine which other components are connected to it.

------

### About the Reusable Component Architecture

The CoreSight reusable component architecture specifies the rules that enable a component to be used with other components that use the CoreSight architecture, and defines the physical interfaces of the components so that they can be connected together easily.

Specification describes following interfaces:

- AMBA APB interface
- AMBA ATB interface
- Event Interface
- Channel interface
- Authentication Interface
- Timestamp Interface

------

### About the System Architecture

The system architecture specifies:

- Rules that must be followed by all systems that implement CoreSight components.
- Additional information that is required by debuggers to use a CoreSight system.



Specification describes system aspects that must be considered when integrating CoreSight components into a system. 

- **Clock** and **power** domains describes the requirements for the clock and power domain structure that is exposed to debuggers.
  - System domain
  - Debug domain
  - Always-on domain
- Control of **authentication** interfaces describes the requirements for the signals in the authentication interface.
- **Memory system** design describes how to expose CoreSight registers to system software (Debug APB interface memory map).



**Topology Detection** describes the topology detection requirements of CoreSight components. Topology Detection at the Component Level describes how to perform topology detection on each interface type. 

When connecting to a CoreSight system, a debugger performs the following steps:
1. It finds the DP.
2. It ensures that the system is powered up, and that its clocks are running. The DP provides facilities to assist with this assessment.
3. It looks for a ROM Table with the location of all components.
4. It compares the Peripheral ID of the ROM Table against a list of saved system descriptions. 
5. If a description of the system with this ID is saved, it uses that description. If not, the debugger continues with the
  following steps:
  1. It identifies each component.
  2. It looks up information that is known about that component to determine what interfaces are supported and how to control them for topology detection.
  3. It performs topology detection.
  4. It saves the description for later use.

------

### About ROM Tables

ROM Tables hold information about debug components.

- Systems with a single debug component do not require a ROM Table. However, a designer might choose to implement such a system to include a ROM Table.
- Systems with more than one debug component must include at least one ROM Table.

A ROM Table connects to a bus controlled by a Memory Access Port (MEM-AP). In other words, the ROM Table is part of the address space of the memory system that is connected to a MEM-AP. More than one ROM Table can be connected to a single bus.

A ROM Table always occupies **4KB** of memory.

There two classes of ROM Tables:

- Class 0x1 ROM Tables
- Class 0x9 ROM Tables

For a detailed description of the ROM Table entries and registers, see Arm® Debug Interface Architecture Specification (ADIv6.0) .

Any ROM Table must implement a set of **Component** and **Peripheral** ID Registers, that start at offset **0xFD0** in the ROM Table.

