# Memory Model

------

### Address Space, Alignment and Endianess

The ARMv6-M architecture uses a single, flat address space of 232 8-bit bytes, covering 4GB.

For ARMv6-M, instruction fetches are always **halfword-aligned** and data accesses are always naturally aligned.

In ARMv6-M the top of memory is defined to always have the **eXecute Never (XN)** memory attribute associated with it.

ARMv6-M always generates a **fault** when an unaligned access occurs. All instruction fetches are **halfword-aligned**.

A memory system uses either a little-endian or a big-endian mapping scheme that defines the endianness of the memory system. In ARMv6-M, it is **IMPLEMENTATION DEFINED** whether the selection of big-endian or little-endian memory mapping is fixed, or is determined from a control input on a reset.

------

### Memory Type Attribute

The following mutually-exclusive main memory type attributes describe the memory regions: Normal, Device, Strongly-ordered.

#### Normal memory

**Normal memory** is idempotent, meaning that it exhibits the following properties:

- read transactions can be repeated with no side effects
- repeated read transactions return the last value written to the resource being read
- read transactions can prefetch additional memory locations with no side effects
- write transactions can be repeated with no side effects, provided that the location is unchanged between the repeated writes
- transactions can be merged prior to accessing the target memory system

Accesses to Normal memory conform to the weakly-ordered model of memory ordering. 

Normal memory is usually  Preprogrammed Flash, ROM, SRAM, SDRAM and DDR memory.

#### Device memory

**Device memory** type attribute defines memory locations where an access to the location can cause side effects, or where the value returned for a load can vary depending on the number of loads performed.

For explicit accesses from the processor to memory marked as Device:

- all accesses occur at their program size
- the number of accesses is the number specified by the program
- architecture does not permit speculative accesses
- software must not use an instruction that generates a sequence of accesses to access Device memory

Memory-mapped peripherals and I/O locations are examples of memory regions normally marked as being Device memory.

#### Strongly-ordered memory

**Strongly-ordered memory** type attribute defines memory locations where an access to the location can cause side effects, or where the value returned for a load can vary depending on the number of loads performed.

For explicit accesses from the processor to memory marked as Strongly-ordered:

- all accesses occur at their program size
- the number of accesses is the number specified by the program
- write buffer no supported
- architecture does not permit speculative accesses
- software must not use an instruction that generates a sequence of accesses to access Device memory

Memory-mapped peripherals and I/O locations are examples of memory regions normally marked as being Strongly-ordered memory.

Address locations in Strongly-ordered memory are not held in a cache, and are always treated as shareable memory locations.

------

### Sharable Attribute 

**Non-shareable attribute** identifies Normal memory that is likely to be accessed only by a single processor. A region of memory marked as Non-shareable Normal does not have any requirement to make the effect of a cache transparent for data or instruction accesses. Software mask use cache maintain operations to if the presence of caches might lead to coherency issues when communicating between the
observers.  

**Shareable memory attribute** describes Normal memory that is expected to be accessed by multiple processors or other system masters. A region of Normal memory with the Sharable attribute is one for which the effect of interposing a cache, or caches, on the memory system is entirely transparent to data accesses in the same Shareability domain. Explicit software management is required to ensure the coherency of instruction caches.

Address locations in Strongly-ordered memory are not held in a cache, and are always treated as shareable memory locations.

In addition to marking a region of **Normal memory** as being shareable or Non-shareable, regions can also be marked as being one of:

- cacheable Write-Through.
- cacheable writeback.
- non-cacheable.

------

### Access rights

Access rights consist of the following classes: rights for **data accesses** and rights for **instruction prefetching**.

Privilege level access controls for **data accesses**. The ARM architecture memory attributes can define that a memory region is:

- not accessible to any accesses.
- accessible only to privileged accesses.
- accessible to privileged and unprivileged accesses.

Privilege level access controls for **instruction accesses**. The ARM architecture memory attributes can define that a region of memory is:

- not accessible for execution, meaning prefetching from addresses in the region must not occur.
- accessible for execution by privileged processes only.
- accessible for execution by privileged and unprivileged processes.

------

### Memory access order

The concept of observability applies to all memory, however, the concept of global observability only applies to shareable memory.

- a write to a location in memory is said to be **observed** by an observer when a subsequent read of the location by the same observer returns the value written by the write.
- a write to a location in memory is said to be **globally observed** for a Shareability domain when a subsequent read of the location by any observer within that Shareability domain that is capable of observing the writereturns the value written by the write.
- a read of a location in memory is said to be **observed** by an observer when a subsequent write to the location by the same observer has no effect on the value returned by the read.
- a read of a location in memory is said to be **globally observed** for a Shareability domain when a subsequent write to the location by any observer within that Shareability domain that is capable of observing the write has no effect on the value returned by the read.

------

### Memory barriers

Memory barrier is the general term applied to an instruction, or sequence of instructions, used to force synchronization events by a processor with respect to retiring load or store instructions.

#### Data Memory Barrier (DMB)

The DMB instruction is a data memory barrier. DMB exhibits the following behavior:

- All explicit memory accesses by instructions occurring in program order before this instruction are globally observed before any explicit memory accesses because of instructions occurring in program order after this instruction are observed.
- The DMB instruction has no effect on the ordering of other instructions executing on the processor.

#### Data Synchronization Barrier (DSB)

The DSB instruction operation acts as a special kind of DMB. The DSB operation completes when all explicit memory accesses before this instruction complete.

#### Instruction Synchronization Barrier (ISB)

The ISB instruction flushes the pipeline in the processor, so that all instructions following the pipeline flush are fetched from memory after the instruction has been completed. In addition, the ISB instruction ensures that any branches that appear in program order after the ISB are always written into the branch prediction logic with the context that is visible after the ISB.