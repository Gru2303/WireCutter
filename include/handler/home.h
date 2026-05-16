#pragma once

#include <stdbool.h>

typedef enum {
    HANDLER_HOME_IDLE = 0,
    HANDLER_HOMING_FAST_APPROACH,
    HANDLER_HOMING_BACKOFF,
    HANDLER_HOMING_SLOW_APPROACH,
    HANDLER_HOMING_DONE
} handler_home_state_t;

void handler_home();

void handler_home_tick();

void handler_reset_home();

handler_home_state_t handler_home_get_state();

bool handler_is_homed();