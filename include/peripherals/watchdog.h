#pragma once

#include <stm32f4xx_hal.h>

#include "error.h"

extern IWDG_HandleTypeDef hiwdg;

void watchdog_init();

void watchdog_tick();