#pragma once

#include <stm32f4xx_hal.h>

#include <gcryslcd/gcryslcd.h>

#include "peripherals/i2c.h"

#define DISPLAY_EN         0b1100

#define DISPLAY_RW_WRITE   0b1000
#define DISPLAY_RW_READ    0b1010

#define DISPLAY_RS_COMMAND 0b1000
#define DISPLAY_RS_DATA    0b1001

#define DISPLAY_ADDR 0x27

extern gcryslcd_t display;