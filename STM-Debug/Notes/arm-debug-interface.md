# ARM Debug Interface (ADI)

------

## Introduction

Logically, the ARM Debug Interface (ADI) consists of:

- A number of registers that are private to the ADI. These are referred to as the Debug Access Port (DAP) Registers
- A means to access the DAP registers
- A means to access the debug registers of the debug components to which the ADI is connected



Because the DAP logically consists of two parts, the Debug Port and Access Ports, it must support two types of access:

- Access to the Debug Port (**DP**) registers. This is provided by Debug Port accesses (**DPACC**).
- Access to Access Port (**AP**) registers. This is provided by Access Port accesses (**APACC**).

An ADI can include multiple Access Ports.

An AP is responsible for accessing debug component registers, such as processor debug logic, ETM and trace port registers. These accesses are made in response to APACC accesses in a manner defined by the AP.



An ARM Debug Interface implementation includes a single Debug Port (DP), that provides the external physical connection to the interface. The ARM Debug Interface specification supports two DP implementations:

- JTAG Debug Port (**JTAG-DP**) - based on the IEEE 1149.1 Test Access Port (TAP) and Boundary Scan Architecture, widely
  referred to as JTAG.
- Serial Wire Debug Port (**SW-DP**) - Serial Wire Debug Port implementation provides a bi-directional serial connection to the ARM Debug Interface. It is based on the CoreSight Serial Wire Interface, and can be implemented as either a synchronous or an asynchronous serial port



An Access Port provides the interface between an ADI and one or more debug components. This specification defines two Access Ports, and the complete description of each of these is summarized in:

- **MEM-AP** - The MEM-AP provides access to one or more debug components. If the MEM-AP connects to more than one debug component then it must also include at least one ROM Table.
- **JTAG-AP** - The JTAG-AP provides a standard JTAG connection to one or more legacy components. The connection between the JTAG-AP and the components is described by the IEEE 1149.1-1990 IEEE Standard Test Access Port and Boundary Scan Architecture.

------

## Accessing Access Ports

The DAP is split into two main control units, the Debug Port (**DP**) and the Access Port (**AP**), and the physical connection to the debugger is part of the DP. 

The DAP supports two types of access, Debug Port (DP) accesses and Access Port (AP) accesses. Because Debug Ports usually have serial interfaces, the methods of making these accesses are kept as short as possible. However, all accesses are 32-bits.

One of the four registers within the DP is the AP Select Register, **SELECT**. This register specifies a particular Access Port, and a bank of four 32-bit words within the register map of that AP.

In any AP access transaction from the debugger, the two address bits **A[3:2]** are decoded to select one of the four 32-bit words from the register bank indicated by the **SELECT** Register.

------

## Debug Port (DP) features

In Debug Port user can configure following features:

1. **Error Responses**

   - Sticky Error flag, **STICKYERR**, in the Control/Status Register - read/write error might be generated if the debugger makes an AP transaction request while the debug power domain is powered down
   - Overrun detection **STICKYORUN** flag in the DP Control/Status Register - enables a user to send blocks of commands to an emulator on a high latency, high throughput connection. These commands should be sent with sufficient in-line delays to make overrun errors unlikely. However the DAP can be programmed so that, if an overrun error occurs. Overrun detection mode is enabled by setting the Overrun Detect bit, **ORUNDETECT**
   - Protocol errors, **WDATAERR** - On the Serial Wire Debug interface, protocol errors can occur, for example because of wire-level errors.

2. **Pushed Compare and Pushed Verify**

   The debugger writes a value as an AP transaction

   1. the DP performs a read from the AP
   2. the DP compares the two values and updates the Sticky Compare flag, STICKYCMP, in the DP
   3. Control/Status register, based on the result of the comparison:
      — pushed compare sets **STICKYCMP** to 1 if the values match
      — pushed verify sets **STICKYCMP** to 1 if the values do not match.

   Whenever the **STICKYCMP** bit is set to 1 in this way, any outstanding transaction repeats are cancelled.

   Pushed operations are enabled using the Transfer Mode bits, **TRNMODE**, in the DP Control/Status Register,

   The DP includes a byte lane mask, so that the compare or verify operation can be restricted to particular bytes in the word. This mask is set using the **MASKLANE** bits

3. **The Transaction Counter**

   A Debug Port must include an AP Transaction Counter, TRNCNT. The Transaction Counter enables a debugger to make a single AP transaction request that generates a sequence of AP transactions. With a MEM-AP access, the AP transaction sequence might generate a sequence of accesses to the connected memory system.

   - For a code download or memory fill operation. For memory fill, the Transaction Counter can be used to repeatedly write a single data value supplied in the initial AP transaction request.
   - With pushed compare or pushed verify operation enabled, the Transaction Counter can be used when reading from the DRW register, to perform a fast search or verify of an area of memory.

4. **System and Debug power and Debug reset control**

   DAP **power** domains:

   - Always-on power domain
   - System power domain (**CSYSPWRUPREQ**, **CSYSPWRUPACK**)
   - Debug power domain (**CDBGPWRUPREQ**, **CDBGPWRUPACK**)

   Debug **Reset** Control (**CDBGRSTREQ**, **CDBGRSTACK**)

   

------

## Access Port (AP) features

There are two types of Access Port defined by this ADI specification:

- Memory Access Port (**MEM-AP**)
- JTAG Access Port (**JTAG-AP**)



#### Memory Access Port

A MEM-AP provides a DAP with access to a memory subsystem. Another way of describing the operation of a MEM-AP is that:

- a debug component implements a memory-mapped abstraction of a set of resources
- the MEM-AP provides AP access to those resources.

The programmer’s model for debug registers is a memory map. The debug registers in a debug component occupy one or more 4KB blocks of address space, and a system might contain several such debug components.

A 4KB block of address space accessible from an Access Port can be referred to as a Debug Register File. A single Access Port can access multiple Debug Register Files.

A ROM Table is a special case of a Debug Register File. It is a 4KB memory block that identifies a system.



MEM-AP **registers**:

- Control/Status Word Register, **CSW** - holds control and status information for the MEM-AP
- Transfer Address Register, **TAR** - holds the address for the next access to the memory system, or set of debug resources, connected to the MEM-AP
- Data Read/Write Register, **DRW** - Writing to the DRW initiates a write to the address specified by the TAR. Reading from the DRW initiates a read from the address specified by the TAR.
- Banked Data Registers, **BD0** to **BD3** - The Banked Data Registers provide direct read or write access to a block of four words of memory, starting at the address specified in the TAR
- Configuration Register, **CFG** - hold information about the configuration of the MEM-AP
- Debug Base Address Register, **BASE** - pointer into the connected memory system. It points to one of:
  - the start of a set of debug registers for the single connected debug component
  - the start of a ROM Table that describes the connected debug components.
- Identification Register, **IDR** - identifies the MEM-AP



User can Auto-incrementing the Transfer Address Register (TAR).

User can configure memory access size using Control/Status Word (CSW) Register.

User can configure AP endians using Configuration Register (CFG)

User can configure byte-laning using Control/Status Word (CSW) Register.

User can enable  Packed transfers for byte and half word transfers



#### JTAG Access Port

The JTAG Access Port is an optional component of a Debug Access Port (DAP). It enables up to eight legacy IEEE 1149.1 JTAG scan chains to be connected to the DAP. Each scan chain can contain any number of TAPs, however ARM Limited recommends that only one TAP is connected to each scan chain. 

An external debugger accesses a JTAG component, connected to a JTAG-AP, through a JTAG scan chain. The debugger accesses this scan chain using APACC accesses to registers in the JTAG-AP. A debugger also has to access JTAG-AP registers to control the JTAG-AP, or to obtain status or identification information from the JTAG-AP.

------

## ROM Tables

ROM Tables are used to hold information about debug components.

- When a Debug Access Port connects to a single debug component, a ROM Table is not required. 
- If a Debug Access Port connects to more than one debug component then the system must include at least one ROM Table.

A ROM Table connects to a bus controlled by a Memory Access Port (MEM-AP). In other words, the ROM Table is part of the address space of the memory system that is connected to a MEM-AP.

A ROM Table is a read-only device. Writes to ROM Table addresses are ignored. A ROM Table always occupies 4KB of memory.

The series of ROM Table entries start at the base address of the ROM Table. The values of these entries depend on the subsystem that is implemented.

Each ROM Table entry:

- describes a single debug component within the system
- consists of four bytes of data

Immediately after the last component entry in the ROM Table there must be a blank ROM Table entry. Thismeans a 32-bit entry with the value 0x00000000.

Any ROM Table must implement a set of Component and Peripheral ID Registers, that start at offset 0xFD0 in the ROM Table.

