#pragma once

#include <stm32f4xx_hal.h>

#include "error.h"

extern I2C_HandleTypeDef hi2c1;

void i2c_init();

void i2c_deinit();