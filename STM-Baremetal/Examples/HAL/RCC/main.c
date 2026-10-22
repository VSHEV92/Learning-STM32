#include <stm32f0xx.h>
#include <stdio.h>

UART_HandleTypeDef huart2;

// Redirect stdlib stream to UART
int _write(int file, char *ptr, int len)  {
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}

void SystemClock_Config(void);

void main() {

    HAL_Init();

    SystemClock_Config();
    
    while(1) {
        printf("STM32F0 AHB clock: %ld Hz\n", HAL_RCC_GetHCLKFreq());
        printf("STM32F0 APB clock: %ld Hz\n", HAL_RCC_GetPCLK1Freq());
        printf("\n");
        HAL_Delay(1000);
    }

}


void SystemClock_Config(void)
{
    RCC_OscInitTypeDef OscInit = {0};

    OscInit.OscillatorType = RCC_OSCILLATORTYPE_HSI;  // Configure HSI 
    OscInit.HSIState = RCC_HSI_ON;                    // Enable HSI
    OscInit.PLL.PLLState = RCC_PLL_ON;                // Enable PLL
    OscInit.PLL.PLLSource = RCC_PLLSOURCE_HSI;        // PLL Source is HSI
    OscInit.PLL.PLLMUL = RCC_PLL_MUL16;               // MUL is 16. PLL Output is 32 MHz
    OscInit.PLL.PREDIV = RCC_PREDIV_DIV4;             // PREDIV is 4. PLL input is 2 MHz
    HAL_RCC_OscConfig(&OscInit);

    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;   // SYSCLK is from PLL 
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;          // AHB DIV is 1. AHB = SYSCLK = 32 MHz    
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;           // APB DIV is 4. APB is 8 MHz
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
}
