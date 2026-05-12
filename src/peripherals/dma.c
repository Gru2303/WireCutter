#include <peripherals/dma.h>

DMA_HandleTypeDef hdma_tim1_up = {0};

void dma_init() {
    __HAL_RCC_DMA2_CLK_ENABLE();

    HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
}

void dma_deinit() {
    __HAL_RCC_DMA2_CLK_DISABLE();
}

