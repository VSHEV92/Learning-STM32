# OpenOCD

------

### 1. Startup

Properly installing OpenOCD sets up your operating system to grant it access to the debug adapters. On Linux, this usually involves installing a file in **/etc/udev/rules.d**, so OpenOCD has permissions. An example rules file that works for many common adapters is shipped with OpenOCD in the **contrib** directory.

To specify one or more different configuration files, use **-f** options. For example:

```
openocd -f config1.cfg -f config2.cfg -f config3.cfg
```

If you find a script for your JTAG adapter, and for your board or target, you may be able to hook up your JTAG adapter then start the server with some variation of one of the following:

```
openocd -f interface/ADAPTER.cfg -f board/MYBOARD.cfg
```



##### OpenOCD flow:

1. OpenOCD starts by processing the configuration commands provided on the command line or, if there were no **-c** command or **-f** **file.cfg** options given, in **openocd.cfg**.
2. At the end of the configuration stage it verifies the JTAG scan chain defined using those commands; your configuration should ensure that this always succeeds.
3. Normally, OpenOCD then starts running as a server. Alternatively, commands may be used to terminate the configuration stage early, perform work (such as updating some flash memory), and then shut down without acting as a server.
4. Once OpenOCD starts running as a server, it waits for connections from clients (Telnet, GDB, RPC) and processes the commands issued through those channels.

------

### 2. Adapter Setup

Correctly installing OpenOCD includes making your operating system give OpenOCD access to debug adapters. Once that has been done, Tcl commands are used to select which one is used, and to configure how it is used.

Debug Adapters are normally configured through commands in an interface configuration file which is sourced by your openocd.cfg file, or through a command line **-f interface/....cfg** option.

```
source [find interface/olimex-jtag-tiny.cfg]
```



##### Most useful options:

- **adapter driver name** - Use the adapter driver name to connect to the target
- **adapter usb vid_pid** - Specifies the USB vendor and product IDs of the adapter to use
- **adapter serial serial string** - Specifies the serial string of the adapter to use.
- **transport select [transport name]** - Select which of the supported transports to use in this OpenOCD session (JTAG, SWD)
- **adapter speed max speed kHz** - A non-zero speed is in KHZ.

------

### 3. Reset Setup

Every system configuration may require a different reset configuration. This can also be quite confusing. Resets also interact with reset-init event handlers, which do things like setting up clocks and DRAM, and JTAG clock rates.

There are many kinds of reset possible through JTAG, but they may not all work with a given board and adapter:

- **System Reset (SRST)** - hardware signal resets all chips connected to the JTAG adapter, such as processors, power management chips, and I/O controllers. Normally resets triggered with this signal behave exactly like pressing a RESET button.
- **JTAG TAP Reset (TRST)** - hardware signal resets just the TAP controllers connected to the JTAG adapter. Such resets should not be visible to the rest of the system; resetting a device’s TAP controller just puts that controller into a known state



##### Most useful options:

- **adapter srst pulse_width milliseconds** - Minimum amount of time (in milliseconds) OpenOCD should wait after asserting nSRST (active-low system reset) before allowing it to be deasserted.
- **adapter srst delay milliseconds** - How long (in milliseconds) OpenOCD should wait after deasserting nSRST (active- low system reset) before starting new JTAG operations.
- **jtag_ntrst_assert_width milliseconds** - Minimum amount of time (in milliseconds) OpenOCD should wait after asserting nTRST (active-low JTAG TAP reset) before allowing it to be deasserted.
- **jtag_ntrst_delay milliseconds** - How long (in milliseconds) OpenOCD should wait after deasserting nTRST (active-low JTAG TAP reset) before starting new JTAG operations.
- **reset_config** - This command displays or modifies the reset configuration of your combination of JTAG board and target in target configuration scripts:
  - signals can specify which of the reset signals are connected (**trst_only**, **srst_only** and **trst_and_srst**)
  - combination is an optional value specifying broken reset signal implementations (**separate**, **trst_pulls_srst**, **srst_pulls_trst**, **combined**)
  - gates tokens control flags that describe some cases where JTAG may be unavailable during reset (**srst_gates_jtag**, **srst_nogate**)
  - connect type tokens control flags that describe some cases where SRST is asserted while connecting to the target (**connect_deassert_srst**, **connect_assert_srst**)
  - trst type driver modes for the test reset signal (**trst_push_pull**, **trst_open_drain**)
  - srst type driver modes for the system reset signal (**srst_open_drain**, **srst_push_pull**)

------

### 4. TAP Setup

TAPs are part of a hardware scan chain, which is a daisy chain of TAPs. They also need to be added to OpenOCD’s software mirror of that hardware list, giving each member a name and associating other data with it. 

Simple scan chains, with a single TAP, are common in systems with a single microcontroller or microprocessor. More complex chips may have several TAPs internally.



#### JTAG TAP

##### TAP Declaration Commands

- **jtag newtap chipname tapname configparams** - Declares a new TAP with the dotted name chipname.tapname, and configured according to the various configparams.
  - The **chipname** is a symbolic name for the chip. Conventionally target config files use $_CHIPNAME, defaulting to the model name given by the chip vendor but overridable.
  - The **tapname** reflects the role of that TAP, and should follow this convention (bs, cpu, flash, etb and so on)
- Every TAP has following **configparams**:
  - **-irlen NUMBER** - The length in bits of the instruction register
  - **-expected-id NUMBER** - A non-zero number represents a 32-bit IDCODE which you expect to find when the scan chain is examined.
- Other TAP commands:
  - **jtag cget dotted.name -idcode** - Get the value of the IDCODE found in hardware
  - **jtag configure dotted.name** -event event name handler - At this writing this TAP attribute mechanism is limited and used mostly for event handling.
  - **scan_chain** - Displays the TAPs in the scan chain configuration, and their status. The set of TAPs listed by this command is fixed by exiting the OpenOCD configuration stage,



#### DAP

Since OpenOCD version 0.11.0, the Debug Access Port (DAP) is no longer implicitly created together with the target. It must be explicitly declared using the **dap create** command for all ARMv6-M, ARMv7 and ARMv8 targets

##### DAP Declaration Commands

- **dap create dap name -tap dotted.name configparams** - Declare a DAP instance named dap name linked to the JTAG tap dotted.name. This also creates a new command (dap_name) which is used for various purposes including additional configuration.
- A DAP may also provide optional **configparams**:
  - **-adiv5** - Specify that it’s an ADIv5 DAP
  - **-adiv6** - Specify that it’s an ADIv6 DAP
  - **-dp-id number** - Debug port identification number for SWD DPv2 multidrop. The number is written to bits 0..27 of DP TARGETSEL during DP selection
  - **-instance-id number** - Instance identification number for SWD DPv2 multidrop. The number is written to bits 28..31 of DP TARGETSEL during DP selection
- Other useful **commands**:
  - **dap info [num|root]** - Displays the ROM table for MEM-AP num, defaulting to the currently selected AP of the currently selected target
  - **dat init** - Initialize all registered DAPs. This command is used internally during initialization.
  - **$dap_name apid [num]** - Displays ID register from AP num, defaulting to the currently selected AP.
  - **$dap_name apreg ap num reg [value]** - Displays content of a register reg from AP ap num or set a new value value. On ADIv5 DAP ap num is the numeric index of the AP.
  - **$dap_name apsel [num]** - Select AP num, defaulting to 0. On ADIv5 DAP num is the numeric index of the AP.
  - **$dap_name dpreg reg [value]** - Displays the content of DP register at address reg, or set it to a new value value.
  - **$dap_name baseaddr [num]** - Displays debug base address from MEM-AP num, defaulting to the currently selected AP. On ADIv5 DAP num is the numeric index of the AP

------

### 5. CPU Setup

All targets that have been set up are part of a list, where each member has a name. That name should normally be the same as the TAP name. You can display the list with the **targets** command.

Before creating a “target”, you must have added its TAP to the scan chain. When you’ve added that TAP, you will have a dotted.name which is used to set up the CPU support. The chip-specific configuration file will normally configure its CPU(s) right after it adds all of the chip’s TAPs to the scan chain.

The two main things to configure after target creation are a work area, which usually has target-specific defaults even if the board setup code overrides them later; and event handlers, which tend to be much more board-specific.

You should specify a working area if you can; typically it uses some on-chip SRAM. Such a working area can speed up many things, including bulk writes to target memory; flash operations like checking to see if memory needs to be erased; GDB memory checksumming; and more.

It’s often very useful to define a reset-init event handler. For systems that are normally used with a boot loader, common tasks include updating clocks and initializing memory controllers.

##### Target Declaration Commands

- **target create target name type configparams** - creates a GDB debug target that refers to a specific JTAG tap
  - **target name** - is the name of the debug target. By convention this should be the same as the dotted.name of the TAP associated with this target
  - **type** - specifies the target type (cortex_m, esp32, riscv and so on)
  - **configparams** - all parameters accepted by **$target_name configure** are permitted.
- Target **configparams**:
  - **-tap dotted.name** – names the TAP used to access this target
  - **-dap dap name** – names the DAP used to access this target
  - **-endian (big|little)** – specifies whether the CPU uses big or little endian conventions
  - **-work-area-backup (0|1)** – says whether the work area gets backed up; by default, it is not backed up
  - **-work-area-size size** – specify work are size, in bytes
  - **-work-area-phys address** – set the work area base address to be used when no MMU is active
  - **-work-area-virt address** – set the work area base address to be used when an MMU is active. Do not specify a value for this except on targets with an MMU.
  - **-rtos rtos type** – enable rtos support for target, rtos type can be one of **auto**,
  - **-ap-num ap number** – set DAP access port for target
  - **-coreid coreid** – set an index to identify the CPU or its debug controller
- Other target commands:
  - All commands to write/read memory and registers (mdw, mww, set_reg, get_reg and so on)
  - **cget** - to get configuration parameter
  - **curstate**, **debug_reason** - to get status (run, hilt, sleep) and debug event
  - **disassemble** - to disassamble code 

------

### 6. Flash Commands

OpenOCD has different commands for NOR and NAND flash; the “**flash**” command works with NOR flash, while the “**nand**” command works with NAND flash. 

This partially reflects different hardware technologies: NOR flash usually supports direct CPU instruction and data bus access, while data from a NAND flash must be copied to memory before it can be used.

Flash configuration steps:

1. Configure via the command **flash bank**
2. Operate on the flash via **flash subcommand**
3. GDB Flashing (Flashing via GDB requires the flash be configured via **flash bank**, and the GDB flash features be enabled)



##### Flash Configuration Commands

- **flash bank name driver base size chip width bus width target [driver options]** - Configures a flash bank which provides persistent storage for addresses from base tobase+size−1. These banks will often be visible to GDB through the target’s memory map.
  - **name** - may be used to reference the flash bank in other flash commands.
  - **driver** - identifies the controller driver associated with the flash bank being declared. This is usually **cfi** for external flash, or else the name of a microcontroller with embedded flash memory.
  - **base** - Base address of the flash chip
  - **chip width** - Width of the flash chip, in bytes; ignored for most microcontroller drivers
  - **bus width** - Width of the data bus used to access the chip, in bytes; ignored for most microcontroller drivers
  - **target** - Names the target used to issue commands to the flash controller
- Other flash information commands:
  - **flash banks** - Prints a one-line summary of each device that was declared using flash bank, numbered from zero.
  - **flash probe num** - Identify the flash, or validate the parameters of the configured flash. Operation depends on the flash type.



##### Erasing, Reading, Writing to Flash

The target device should be in well defined state before the flash programming begins. Always issue **reset init** before. Do not issue another **reset** or **reset halt** or **resume** until the programming session is finished.

One feature distinguishing NOR flash from NAND or serial flash technologies is that for read access, it acts exactly like any other addressable memory.

Write access works differently. Flash memory normally needs to be erased before it’s written. Erasing a sector turns all of its bits to ones, and writing can turn ones into zeroes.

- **Erase Commands:**
  - **flash erase_sector num first last** - Erase sectors in bank num, starting at sector first up to and including last.
  - **flash erase_address [pad] [unlock] address length** - Erase sectors starting at address for length bytes.
- **Write Commands:**
  - **flash fill(w) address word length** - Fills flash memory with the specified double-word (64 bits), word (32 bits), halfword (16 bits), or byte (8-bit) pattern, starting at address and continuing for length units (word/halfword/byte)
  - **flash write_bank num filename [offset]** - Write the binary filename to flash bank num, starting at offset bytes from the beginning of the bank.
  - **flash write_image [erase] [unlock] filename [offset] [type]** - Write the image filename to the current target’s flash bank(s). Only loadable sections from the image are written. A relocation offset may be specified, in which case it is added to the base address for each section in the image.
- **Read Commands:**
  - **flash md(w) addr [count]** - Display contents of address addr, as 32-bit words (mdw), 16-bit halfwords (mdh), or 8-bit bytes (mdb). If count is specified, displays that many units.
  - **flash read_memory address width count** - This function provides an efficient way to read flash memory from a Tcl script.
  - **flash read_bank num filename [offset [length]]** - Read length bytes from the flash bank num starting at offset and write the contents to the binary filename.
- **Check Commands:**
  - **flash verify_bank num filename [offset]** - Compare the contents of the binary file filename with the contents of the flash bank num starting at offset. If offset is omitted, start at the beginning of the flash bank. Fail if the contents do not match.
  - **flash verify_image filename [offset] [type]** - Verify the image filename to the current target’s flash bank(s). Parameters follow the description of ’flash write image’.
  - **flash erase_check num** - Check erase state of sectors in flash bank num, and display that status.
- **Other Commands:**
  - **flash info num [sectors]** - Print info about flash bank num, a list of protection blocks and their status. Use sectors to show a list of sectors instead
  - **flash protect num first last (on|off)** - Enable (on) or disable (off) protection of flash blocks in flash bank num, starting at protection block first and continuing up to and including last.
  - flash padded_value num value - Sets the default value used for padding any image sections, This should normally match the flash bank erased value.

------

### 7. Image Programming

OpenOCD implements numerous ways to program the target flash, whether internal or external.

To simplify using the flash commands directly a Tcl script is available that handles the programming and verify stage. OpenOCD will program/verify/reset the target and optionally shutdown.

**program filename [preverify] [verify] [reset] [exit] [offset]** - This is a helper script that simplifies using OpenOCD as a standalone programmer. The only required parameter is filename, the others are optional.

The script is executed as follows and by default the following actions will be performed.
1. ’**init**’ is executed.
2. ’**reset init**’ is called to reset and halt the target, any ’reset init’ scripts are executed.
3. **flash write_image** is called to erase and write any flash using the filename given.
4. If the **preverify** parameter is given, the target is "verified" first and only flashed if
this fails.
5. **verify_image** is called if verify parameter is given.
6. **reset run** is called if reset parameter is given.
7. OpenOCD is **shutdown** if exit parameter is given.

------

### 8. General Commands

- **Server Commands:**
  - **help**, **usage**
  - **exit**, **shutdown**
  - **sleep msec**
- **Read/Write Commands**:
  - **reg** - Access a single register by number or by its name.
  - **set_reg**, **get_reg**
  - **write_memory**, **read_memory**
  - **mdd**, **mdw**, **mdh**, **mdb**
  - **mwd**, **mww**, **mwh**, **mwb**
- **Target State Commands**:
  - **debug_reason**
  - **halt**, **wait_halt**
  - **resume**
  - **step**
  - **reset** (**reset run**, **reset halt**, **reset init**)
  - **adapter assert**, **adapter deassert** - force SRST or TRST signals
- **Image loading Commands:**
  - **dump_image filename address size** - Dump size bytes of target memory starting at address to the binary file named filename
  - **load_image filename [address [bin|ihex|elf|s19 [min_addr [max_length]]]]** - Load image from file filename to target memory
  - **test_image filename [address [bin|ihex|elf]]** - Displays image section sizes and addresses as if filename were loaded into target memory starting at address
  - **verify_image filename [address [bin|ihex|elf]]** - Verify filename against target memory. If an address is specified, it is used as an offset to the file format defined addressing
- **Breakpoint and Watchpoint Commands:**
  - **bp [address [asid] len [hw | hw_ctx]]** - With no parameters, lists all active breakpoints. Else sets a breakpoint on code
    execution starting at address for length bytes.
  - **wp [address length [(r|w|a) [value [mask]]]]** - With no parameters, lists all active watchpoints. Else sets a data watchpoint on data from address for length bytes. The watch point is an "access" watchpoint unless the r or w parameter is provided, defining it as respectively a read or write watchpoint.
  - **rbp all | address** - Remove the breakpoint at address or all breakpoints
  - **rwp all | address** - Remove data watchpoint on address or all watchpoints

------



