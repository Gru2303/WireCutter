#pragma once

#include <stm32f4xx_hal.h>

#include <peripherals/dma.h>
#include "error.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

void timer_init();

void timer_deinit();