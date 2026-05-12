#pragma once

#include <stm32f4xx_hal.h>

#include <stdint.h>

#include "stm32f4xx_hal_conf.h"
#include "interupts.h"
#include "error.h"

#include "peripherals/clock.h"
#include "peripherals/gpio.h"
#include "peripherals/dma.h"
#include "peripherals/timer.h"
#include "peripherals/dwt.h"
#include "peripherals/i2c.h"
#include "peripherals/watchdog.h"
#include "controls/buttons.h"
#include "controls/encoder.h"
#include "display/display.h"
#include "display/menu.h"

#include "gstepper/gstepper.h"

extern  void setup();
extern void loop();