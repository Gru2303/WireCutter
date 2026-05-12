#pragma once

#include <stm32f4xx_hal.h>

void dwt_init();

void dwt_deinit();

uint32_t dwt_get_tick();

uint32_t dwt_get_micros();