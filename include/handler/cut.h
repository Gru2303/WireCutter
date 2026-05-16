#pragma once

#include <stdbool.h>
#include <gcryslcd/gcryslcd.h>

#include "home.h"
#include "motors/motors.h"
#include "menu/menu.h"

typedef enum {
    HANDLER_CUTTING_STATE_IDLE = 0,

    HANDLER_CUTTING_STATE_HOMING,
    HANDLER_CUTTING_STATE_PREPARATION,

    HANDLER_CUTTING_STATE_FEED_STRIP_1,
    HANDLER_CUTTING_STATE_PARTIAL_CUT_1,
    HANDLER_CUTTING_STATE_RAISE_BLADE_1,

    HANDLER_CUTTING_STATE_FEED_MAIN,
    HANDLER_CUTTING_STATE_PARTIAL_CUT_2,
    HANDLER_CUTTING_STATE_RAISE_BLADE_2,

    HANDLER_CUTTING_STATE_FEED_STRIP_2,
    HANDLER_CUTTING_STATE_FULL_CUT,
    HANDLER_CUTTING_STATE_RAISE_BLADE_3,

    HANDLER_CUTTING_STATE_CHECK_COUNT,

    HANDLER_CUTTING_STATE_STOP
} handler_cutting_state_t;

typedef struct  {
    int32_t wire_strip_start;
    int32_t wire_length;
    int32_t wire_strip_end;
    int32_t start_cut_pos;
    int32_t partial_cut_pos;
    int32_t full_cut_pos;

    uint32_t count;

    gcryslcd_menu_page_t *end_page;
} handler_cutting_settings_t;

void handler_cutting_start(handler_cutting_settings_t settings);

void handler_cutting_tick();

bool handler_cutting_is_running();

void handler_cutting_stop();

uint32_t handler_cutting_get_count();

uint32_t handler_cutting_get_count_done();