#pragma once

#include <stm32f4xx_hal.h>

extern DMA_HandleTypeDef hdma_tim1_up;

void dma_init();

void dma_deinit();