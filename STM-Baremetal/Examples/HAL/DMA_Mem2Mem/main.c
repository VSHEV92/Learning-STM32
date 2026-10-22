#include <stm32f0xx.h>
#include <stdlib.h>
#include <stdio.h>

DMA_HandleTypeDef hdma_mem2mem;
UART_HandleTypeDef huart2;

int tx_data[] = {1, 2, 3, 4, 5};
int* rx_data;

#define ARRAY_SIZE ( sizeof(tx_data)/sizeof(tx_data[0])  )

// Redirect stdlib stream to UART
int _write(int file, char *ptr, int len)  {
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}


void main() {

    HAL_Init();
    
    while(1) {
        
        rx_data = calloc( ARRAY_SIZE, sizeof(tx_data[0]) );

        HAL_DMA_Start(&hdma_mem2mem, (uint32_t)tx_data, (uint32_t)rx_data, ARRAY_SIZE);
        HAL_DMA_PollForTransfer(&hdma_mem2mem, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);

        for (int i = 0; i < ARRAY_SIZE; i++) {
            printf("%d ", rx_data[i]);
        }
        printf("\n");

        free(rx_data);

        HAL_Delay(500);
    }

}
