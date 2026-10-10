# CMSIS Device

------

This is description for **stm32f042x6** files.



### startup_"device".s

This is startup code for given MCU:

- Define **Vector Table** for system exception and extermal interruts
- Define each ISR handler with weak attribute. Define default implementation for **Default_Handler** for each ISR in **Vector Table**.  **Default_Handler** just busy loop with one instruction
- Define **Reset_Handler**. This is first code executed by MCU. This code do following things:
  - Set **SP** register to value from **Vector Table**. This action performs automatically after **Reset**. But in some cases MCU can boot from **System Flash** memory, that contaion STM32 bootloader. Bootloader can jump to application code, so **SP** is also set manually.
  - Then fucntion **SystemInit** is called. This function may be used to setup clocks, external memory interfaces, to enable co-processors or configure power regulators. This function implemented in **system_"device".c** file 
  - Next code checks **ResetHandler** address. Again Application code can be started by STM32 bootloader. In this case Reset Handler address will be in **Flash System** memory. If this is the case startup code remap address space to **Main Flash** memory so that application can user it's reset vector.
  - Next  start loop that fill **Data Section** in RAM with values from Flash. This section contain initialised  global variables.
  - Next  start loop that fill **BSS Section** in RAM with zeros.  This section contain uninitialised  global variables.
  - Next function **__libc_init_array** called. This function is used to call constructios of statically created objects. This is needed if application is writen in C++.
  - Next startup code jump to application function **main**.
  - If application return from main function startup code stack processor in infinite busy loop **LoopForever**.

 

### system_"device".h

This is header file for some common CMSIS and MCU function and variables. This file is includeed by **"concrete_device.h"** file. It contain prototypes for following features:

- Declaration of **SystemCoreClock** variable that specify current CPU clock frequency (AHB clock - HCLK).
- Declaration of **AHBPrescTable**. This is array of prescaler values for AHB clock (**HCLK**). This is prescaler that divide system clock **SYSCLK**.
- Declaration of **APBPrescTable**. This is array of prescaler values for APB clock (**PCLK**). This is prescaler that divide AHB clock **HCLK**.
- Declaration of **SystemInit** function. This function is called at the beginning of ResetHandler and may be used to setup clocks, external memory interfaces, to enable co-processors, configure power regulators and so on
- Declaration of **SystemCoreClockUpdate** function. This fucntion is needed to update **SystemCoreClock** variable when MCU changes clock configuration, for example change prescalers and PLL configuration or switch clock sources.



### system_"device".c

This is source file for some common CMSIS and MCU function and variables.

- If not defined earlier defines clock frequencies for **HSI**, **HSE** and **HCI48** clocks
- Define **SystemCoreClock** variable that specify current CPU clock frequency (AHB clock - HCLK).
- Define **AHBPrescTable**. This is array of prescaler values for AHB clock (**HCLK**). This is prescaler that divide system clock **SYSCLK**.
- Define **APBPrescTable**. This is array of prescaler values for APB clock (**PCLK**). This is prescaler that divide AHB clock **HCLK**.
- Define **SystemInit** function. This function is called at the beginning of ResetHandler and may be used to setup clocks, external memory interfaces, to enable co-processors, configure power regulators and so on. For **STM32F0** this function do nothing and is empty.
- Define **SystemCoreClockUpdate** function. This fucntion is needed to update **SystemCoreClock** variable when MCU changes clock configuration, for example change prescalers and PLL configuration or switch clock sources. For **STM32F0**  this function do following:
  - Get system clock source: HSI, HSE or PLL
  - If clock source is PLL that code gets PLL source and multiply and divide values. Based on this values system clock frequency is calculated
  - System clock (**SYSCLK**) divided by AHB prescaler value. So we get AHB clock (**HCLK**) and  **SystemCoreClock** is updated



### "concrete_device".h - stm32f042x6.h

This file contain full description of MCU peripheral:

- Some needed defines:
  - **__CM0_REV** - Cortex-M0 CPU revision
  - **__MPU_PRESENT** - is CPU has MPU
  - **__NVIC_PRIO_BITS** - number of priority bit in NVIC. This value is used by NVIC function in CMSIS Core files.
  - **__Vendor_SysTickConfig** - is vendor define it's on version of **SysTickConfig** function. If not default function from CMSIS Core is used
- Define all exception numbers as enumeration of type **IRQn_Type**. This data type is used by NVIC function in CMSIS Core files.
- Include CMSIS Core **core"CPU".h** and CMSIS Device system"device".h headers
- Define structures data types with registers declaration for each periperal of MCU. For example: **ADC_TypeDef** - for ADC, **DMA_Channel_TypeDef** - for DMA channel registers, **GPIO_TypeDef** - for GPIO and so on.
- Define base addresses for each periperal of MCU. For example **FLASH_BASE**, **SPI2_BASE**, **DMA1_Channel1_BASE**
- Using base addresses define pointers to structures of registers for each periperal of MCU. For example **TIM2**, **GPIOA**, **USART1**
- Define register fields masks and positions for each periperal of MCU. For example **ADC_CFGR1_ALIGN_Pos**, **GPIO_OSPEEDR_OSPEEDR13_Pos**, **RCC_CFGR_HPRE_Msk**. Also define some usefull constants for some peripheral. For example **RCC_CFGR_SWS_HSE** - value to select HSE source as system clock
- Define helper macros to check if pointer is valid peripheral pointer. For example **IS_DMA_ALL_INSTANCE**, **IS_IWDG_ALL_INSTANCE** 
- At the end define some aliases for better code portability



### "device".h - stm32f0xx.h

This file need to be included by application.

- Include **"concrete_define".h** header file base of some defines. For example include **stm32f042x6.h** flie if **STM32F042x6** is defines
- Define STM32 MCU version. For example defines **__STM32F0_DEVICE_VERSION_MAIN**, **__STM32F0_DEVICE_VERSION_RC**
- Define some usefull enums:
  - **FlagStatus** - SET, RESET
  - **FunctionalState** - ENABLE, DISABLE
  - **ErrorStatus** - SUCCESS, ERROR 
- Define some macros for register bits and fields manipulation, for example **SET_BIT**, **WRITE_REG**, **MODIFY_REG** and so on. This macros is used in STM32 LL Drivers
- If **USE_HAL_DRIVER** is defined then include STM32 HAL header file. For example **stm32f0xx_hal.h**