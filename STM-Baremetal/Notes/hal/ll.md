# Low-Layer Drivers

------

## Abstract

The low-layer (LL) drivers are designed to offer a fast light-weight expert-oriented layer which is closer to the hardware than the HAL. Contrary to the HAL, LL APIs are not provided for peripherals where optimized access is not a key feature, or those requiring heavy software configuration and/or complex upper-level stack (such as USB).

The low-layer drivers provide hardware services based on the available features of the STM32 peripherals. These services reflect exactly the hardware capabilities and provide one-shot operations that must be called following the programming model described in the microcontroller line reference manual. As a result, the LL services do not implement any processing and do not require any additional memory resources to save their states, counter or data pointers: all the operations are performed by changing the associated peripheral registers content.

------

## Low-layer files

- **stm32f0xx_ll_cortex.h** = Cortex-M related register operation APIs including the **Systick**, Low power, CPUID



- **stm32f0xx_ll_bus.h** - This is the h-source file for core bus control and peripheral clock activation and deactivation

  - Need **RCC** peripheral to be defined
  - Macros to enable and disable clocks of **AHB** and **APB** peripheral
  - Macros to assert and deassert resets of **AHB** and **APB** peripheral

  

- **stm32f0xx_ll_system.h** - System related operations.

  - Need **FLASH**, **SYSCONFIG** or  **DBGMCU** peripheral to be defined
  - Macros to access **SYSCONFIG** registers 
    - Memory and DMA remap
    - External interrupts configuration
    - PVD level configuration
    - RAM parity
    - Lockup Flag
  - Clocks behavior (stop or run) in Debug mode setup
  - Flash latency and Prefetch configuration

  

- **stm32f0xx_ll_utils.h** - header that declare useful utils:

  - Define **LL_MAX_DELAY** for delay functions
  - Define Flash size
  - **LL_UTILS_PLLInitTypeDef** - configuration structure for PLL (multiplication and division factor). Use to setup System Clock (SYSCLK)
  - **LL_UTILS_ClkInitTypeDef** - configuration structure for AHB and APB clocks
  - Macros to get device UID and device signature
  - Macros to init and configure SysTick:
    - **LL_InitTick** - get HCLK and period in number of ticks
    - **LL_Init1msTick** - get HCLK and set period to 1 ms
    - **LL_mDelay** - busy loop delay in number of ms 
  - Functions to configure System Clock:
    - LL_PLL_ConfigSystemClock_HSI
    - LL_PLL_ConfigSystemClock_HSI48
    - LL_PLL_ConfigSystemClock_HSE
  - Setup Flash Latency function **LL_SetFlashLatency**

- **stm32f0xx_ll_utils.c** - source file that define useful utils and device parameters:

  - Define min/max values for clock:
    - For PLL output
    - For HSE
    - Frequency when flash latency need to be switched
  - Checkers macro to correct clock division values
  - Define functions to configure System Clock:
    - LL_PLL_ConfigSystemClock_HSI
    - LL_PLL_ConfigSystemClock_HSI48
    - LL_PLL_ConfigSystemClock_HSE

- **stm32f0xx_ll_"periph".h/c** - header and source files that declare useful functions and defines for concrete peripheral:

  - **RCC** - files for Reset and Clock Control systems
    - Default frequencies for HSI, HSE, LSI, LSE, HSI48, HSI14
    - Defines for each register's field supported values
    - Macros to configure each RCC register's field (PLL, dividers, set/clear flags, clock sources selector, clock enable/disable/ready flags)
    - **RCC_DeInit()**  - restore defaul reset values for all RCC registers
    - Functions to get System, AHB and APB frequency:
      - RCC_GetSystemClockFreq
      - RCC_GetHCLKClockFreq
      - RCC_GetPCLK1ClockFreq
    - Functions to get frequency for peripheral with switched clock sources:
      - LL_RCC_GetCECClockFreq
      - LL_RCC_GetUSBClockFreq
      - LL_RCC_GetI2CClockFreq
      - LL_RCC_GetUSARTClockFreq
  - **PWR** -  files for Power Control systems
    - Marcos and defines set, reset and read Wake UP flags
    - Flags for Stop and Standby modes
    - Flags for Main and Low Power Stop modes
    - PVD levels
    - Macros to configure each RCC register's field (power modes, backup region access, voltage detectors enable/disable, voltage thresholds)
    - **PWR_DeInit()**  - force and release reset of PWR peripheral mode
  - **Other peripheral** - file for other available peripheral (GPIO, USART, I2C and so on):
    - Macros to configure each register's field 
    - **LL_"Periph"_InitTypeDef** - peripheral configuration structure. Contain high level configuration for all registers in one place 
    - **LL_"Periph"_ClockInitTypeDef** - peripheral clock configuration structure. Contain high level configuration for all clock control registers in one place 
    - **LL_"Periph"_DeInit** - restore default reset values for all peripheral registers
    - **LL_"Periph"_Init** - initialize peripheral using initialization configuration structure
    - **LL_"Periph"_StructInit** - initialize configuration structure to default reset values

------

## Using Rules



### Peripheral initialization functions

The LL drivers offer three sets of initialization functions. They are defined in **stm32f0xx_ll_"periph".c** file:

- Functions to initialize peripheral main features according to the parameters specified in data structures
- A set of functions used to fill initialization data structures with the reset values of each field
- Function for peripheral de-initialization (peripheral registers restored to their default values)

The definition of these LL initialization functions and associated resources (structure, literals and prototypes) is conditioned by a compilation switch: **USE_FULL_LL_DRIVER**. To use these functions, this switch must be added in the toolchain compiler preprocessor or to any generic header file which is processed before the LL drivers.



### Run-time checking

Like HAL drivers, LL initialization functions implement run-time failure detection by checking the input values of all LL driver functions. When using the LL drivers in standalone mode (without calling HAL functions), the following actions are required to use run-time checking:

1. Copy **stm32_assert_template.h** to the application folder and rename it to **stm32_assert.h**. This file defines the assert_param macro which is used when run-time checking is enabled.
2. Include **stm32_assert.h** file within the application main header file.
3. Add the **USE_FULL_ASSERT** compilation switch in the toolchain compiler preprocessor or in any generic header file which is processed before the **stm32_assert.h** driver.

