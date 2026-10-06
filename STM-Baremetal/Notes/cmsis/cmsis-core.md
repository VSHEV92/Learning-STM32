# CMSIS Core

------

## Introduction

The **CMSIS** is a set of tools, APIs, frameworks, and work flows that help to simplify software re-use, reduce the learning curve for microcontroller developers, speed-up project build and debug, and thus reduce the time to market for new applications.

CMSIS has been created to help the industry in standardization. It enables consistent software layers and device support across a wide range of development tools and microcontrollers. CMSIS is not a huge software layer that introduces overhead and does not define standard peripherals. The silicon industry can therefore support the wide variations of Arm Cortex processor-based devices with this common standard.

------

## Dependences

CMSIS Core repository have some dependences from vendor:

- **startup_"device".c** - has reset handler and exception vectors.
- **system_"device".c** - has general device configuration (i.e. for clock and BUS setup).
- **system_"device".h** - contain device prototypes and defines
- **"device".h** - file need to be included by application

This file define some features without which CMSIS Core does not build:

- **__NVIC_PRIO_BITS** - (define) number of bits for handler priority 
- **IRQn_Type** - (enum) set exception numbers

------

## Source Files



### m-profile/armv7m_mpu.h

- Define encodings for region sizes
- Define encodings for access permissions
- Define macros to set values MPU **RBAR** and **RASR** registers ( **ARM_MPU_RBAR()**, **ARM_MPU_RASR()** )
- Structure type for MPU region **ARM_MPU_Region_t**
- Function to configure MPU
  - **ARM_MPU_Enable**, **ARM_MPU_Disable**
  - **ARM_MPU_ClrRegion**, **ARM_MPU_SetRegion** 
  - **ARM_MPU_Load** - load regions from table 



### m-profile/armv7m_cachel1.h

- Define macros to access cache ID registers: **CCSIDR_WAYS**, **CCSIDR_SETS** 
- Define macros to get line sizes: **__SCB_DCACHE_LINE_SIZE**, **__SCB_ICACHE_LINE_SIZE**
- Define function to control cache:
  - **SCB_EnableICache**, **SCB_DisableICache**, **SCB_InvalidateICache**
  - **SCB_EnableDCache**, **SCB_DisableDCache**, **SCB_InvalidateDCache**, **SCB_CleanDCache**



### m-profile/cmsis_gcc_m.h

- Depending on C library rename or define **_start** function. Generic definition has copy table loop and zero table loop. 
- Add some define like **__PROGRAM_START** as **__cmsis_start**, **__INITIAL_SP** as **__StackTop**,  **__STACK_LIMIT** as **__StackLimit**,  **__VECTOR_TABLE_ATTRIBUTE** as **__attribute__((used, section(".vectors")))** and other. This need for code portability to other compilers.
- Define function to access special regiters:
  - **__get_CONTROL()**, **__set_CONTROL()**
  - **__get_PRIMASK()**, **__set_PRIMASK()**
  - **__get_FAULTMASK()**, **__set_FAULTMASK()**
  - **__get_BASEPRI()**, **__set_BASEPRI()**
  - **__get_IPSR()**, **__set_IPSR()** 
  - **__get_APSR()**, **__set_APSR()** 
  - **__get_xPSR()**, **__set_xPSR()** 
  - **__get_PSP()**, **__set_PSP()** 
  - **__get_MSP()**, **__set_MSP()** 
- Define non-secure version of this function to TrustZone
- Add function to access ARM-8M special register



### cmsis_gcc.h

- Rename many compiler attributes and modifications. For example **inline** to **__INLINE**,  **__attribute__(( __noreturn__))** to **__NO_RETURN** and so on. This need for code portability to other compilers.
- Define unaligned memory access macros, like **__UNALIGNED_UINT16_WRITE** or **__UNALIGNED_UINT16_READ(addr)**
- Define functions for instructions that can not be called from C code. For example **__NOP()**, **__WFI()**, **__ROR()**, **__ISB()** and so on. This is only for Application Level Architecture part. Instructions for all ARM profiles (A, R, M).
- Define some common useful function for interrupts and control registers. For example:
  - **__enable_irq**, **__disable_irq**
  - **__enable_fault_irq**, **__disable_fault_irq**
  - **__get_FPSCR()**, **__set_FPSCR()**
- Rename DSP instruction to intrinsic function. For example **__SADD8** to **__sadd8**, **__SHSUB16** to **__shsub16** and so on. This need for code portability to other compilers.
- Define some DSP functions, like **__PKHTB**, **__SXTAB16_RORn** and other.
- Include profile specific header file. For example **m-profile/cmsis_gcc_m.h**



### cmsis_compiler.h

- Include need header file base on compiler. For example include **cmsis_gcc.h** for GCC
- For some complier like **TASKING** and **COSMIC** define and rename features that doing in separate headers for other compilers



### cmsis_version.h

- Add define for CMSIS main version: **__CM_CMSIS_VERSION_MAIN**
- Add define for CMSIS sub version: **__CM_CMSIS_VERSION_SUB**
- Add define for CMSIS main/sub version: **__CM_CMSIS_VERSION**



### cmsis_cm0.h

- include **cmsis_version.h** file
- Add some useful defines:
  - **__CORTEX_M** - processor version. For example 0 for Cortex-M0
  - **__FPU_USED** - is FPU used
  - Sysadd compiler checks for **__FPU_PRESENT**
- include **cmsis_compiler.h** file
- checks and define for some defines base on **__CHECK_DEVICE_DEFINES** flag:
  - **__CM0_REV** - core revision
  - **__NVIC_PRIO_BITS** - number of priority bits
  - **__Vendor_SysTickConfig** - is there SysTickConfig function in vendor CMSIS Device files
- Add aliases for **volatile** keyword: **__I**, **__O**,  **__IO**,  **__IOM** and so on.
- Declare structures for special registers and position and mask defines for there fields: **APSR_Type**, **APSR_N_Pos**,  **IPSR_Type**, **IPSR_ISR_Msk**, **CONTROL_Type**, **CONTROL_SPSEL_Pos** and so on.
- Declare structures for NVIC registers: **NVIC_Type**
- Declare structures for System Control Block (**SCB**) registers and position and mask defines for there fields: **SCB_Type**, **SCB_CPUID_ARCHITECTURE_Pos**, **SCB_ICSR_NMIPENDSET_Pos**, **SCB_ICSR_VECTACTIVE_Msk**, **SCB_AIRCR_SYSRESETREQ_Pos**, **SCB_CCR_UNALIGN_TRP_Msk**, **SCB_SHCSR_SVCALLPENDED_Pos** and so on
- Declare structures for SysTick registers and position and mask defines for there fields: **SysTick_Type**, **SysTick_CTRL_CLKSOURCE_Pos**, **SysTick_LOAD_RELOAD_Msk**, **SysTick_CALIB_SKEW_Pos** and so on
- Define two helper macros: **_VAL2FLD(field, value)**, **_FLD2VAL(field, value)**  
- Define **SCB**, **SysTick** and **NVIC** base addresses. Also define pointers to these addresses.
- Define NVIC functions:
  - **NVIC_SetPriorityGrouping()**, **NVIC_GetPriorityGrouping()**
  - **NVIC_EnableIRQ()**, **NVIC_DisableIRQ()**
  - **NVIC_GetPendingIRQ()**, **NVIC_SetPendingIRQ()**, **NVIC_ClearPendingIRQ()**
  - **NVIC_SetPriority()**,  **NVIC_GetPriority()** 
  - **NVIC_SystemReset()**
- Add defines for Exception Return Values: **EXC_RETURN_HANDLER**, **EXC_RETURN_THREAD_MSP**, **EXC_RETURN_THREAD_PSP**
- Add function to set and vector handlers: **__NVIC_SetVector()** and **__NVIC_GetVector()**
- If vendor does not define **SysTick_Config()** function then define default implementation
- 





