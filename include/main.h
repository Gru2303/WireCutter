#pragma once

#include <stm32f4xx_hal.h>

#include <stdint.h>

#include "stm32f4xx_hal_conf.h"
#include "interupts.h"
#include "error.h"

#include "peripherals/clock.h"
#include "peripherals/gpio.h"
#include "peripherals/timer.h"
#include "peripherals/dwt.h"
#include "peripherals/i2c.h"
#include "peripherals/watchdog.h"
#include "controls/buttons.h"
#include "controls/encoder.h"
#include "display/display.h"
#include "menu/menu.h"
#include "motors/motors.h"

#include "handler/home.h"
#include "handler/cut.h"

extern  void setup();
extern void loop();