#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

#ifndef GSTEPPER_FAST_PROFILE
    #define GSTEPPER_FAST_PROFILE 10
#endif

typedef enum {
    GSTEPPER_DIR_RIGHT = 1,
    GSTEPPER_DIR_NONE = 0,
    GSTEPPER_DIR_LEFT = -1
} gstepper_dir_t;

typedef enum {
    GSTEPPER_STATE_STOP = 0,
    GSTEPPER_STATE_TARGET = 1,
    GSTEPPER_STATE_PAUSE = 2,
    GSTEPPER_STATE_SPEED = 3,
    GSTEPPER_STATE_SMOOTH_STOP = 4,
} gstepper_state_t;

typedef enum {
    GSTEPPER_TARGET_MODE_ABSOLUTE = 0,
    GSTEPPER_TARGET_MODE_RELATIVE
} gstepper_target_mode_t;

typedef struct {
    void (*step)();
    void (*dir)(gstepper_dir_t dir);

    void (*enable)();
    void (*disable)();

    bool (*can_move)(gstepper_dir_t dir);
} gstepper_init_t;

typedef struct {
    bool init;

    int32_t pos;
    gstepper_dir_t dir;
    bool enabled;
    bool auto_power;

    gstepper_state_t state;

    int32_t min_pos;
    int32_t max_pos;
    bool has_limits;

    uint16_t acceleration;
    uint16_t max_speed;

    uint32_t timer;
    uint32_t period;
    uint32_t period_min;

    int32_t target_pos;
    bool is_target_reached;

    bool settings_changed;
    bool is_speed_zero;
    uint32_t pending_min_period;
    uint16_t pending_acceleration;

    int16_t smooth_stop_steps;
    uint32_t start_period;
    uint32_t period_scaled;
    int32_t total_move_steps;
    int32_t accel_end_step;
    int32_t decel_start_step;
    int32_t start_step_offset;
    int32_t steps;

    int32_t buffered_target_pos;
    bool is_reversing;

    uint32_t fast_profile_steps[GSTEPPER_FAST_PROFILE];
    uint32_t fast_profile_periods[GSTEPPER_FAST_PROFILE];
} gstepper_internal_t;

typedef struct {
    gstepper_init_t init;

    gstepper_internal_t _internal;
} gstepper_t;

extern uint32_t gstepper_get_micros();

void gstepper_init(gstepper_t *stepper);

void gstepper_deinit(gstepper_t *stepper);

bool gstepper_is_init(gstepper_t *stepper);

bool gstepper_can_move_in_dir(gstepper_t *stepper, gstepper_dir_t dir);

bool gstepper_can_move(gstepper_t *stepper);

void gstepper_step(gstepper_t *stepper);

void gstepper_set_dir(gstepper_t *stepper, gstepper_dir_t dir);

gstepper_dir_t gstepper_get_dir(gstepper_t *stepper);

gstepper_dir_t gstepper_toggle_dir(gstepper_t *stepper);

void gstepper_enable(gstepper_t *stepper);

void gstepper_disable(gstepper_t *stepper);

bool gstepper_is_enabled(gstepper_t *stepper);

void gstepper_enable_auto_power(gstepper_t *stepper);

void gstepper_disable_auto_power(gstepper_t *stepper);

bool gstepper_is_auto_power_enabled(gstepper_t *stepper);

void gstepper_set_pos(gstepper_t *stepper, int32_t pos);

int32_t gstepper_get_pos(gstepper_t *stepper);

void gstepper_reset_pos(gstepper_t *stepper);

void gstepper_set_limits(gstepper_t *stepper, int32_t min, int32_t max);

void gstepper_disable_limits(gstepper_t *stepper);

bool gstepper_has_limits(gstepper_t *stepper);

gstepper_state_t gstepper_tick(gstepper_t *stepper);

gstepper_state_t gstepper_tick_manual(gstepper_t *stepper);

gstepper_state_t gstepper_get_state(gstepper_t *stepper);

bool gstepper_ready(gstepper_t *stepper);

void gstepper_set_target(gstepper_t *stepper, int32_t target, gstepper_target_mode_t relative);

void gstepper_set_target_with_speed(gstepper_t *stepper, int32_t target, gstepper_target_mode_t relative, int32_t speed);

int32_t gstepper_get_target(gstepper_t *stepper);

bool gstepper_set_speed(gstepper_t *stepper, int32_t speed);

int32_t gstepper_get_speed(gstepper_t *stepper);

void gstepper_set_max_speed(gstepper_t *stepper, int32_t max_speed);

void gstepper_set_acceleration(gstepper_t *stepper, int32_t accel);

void gstepper_brake(gstepper_t *stepper);

void gstepper_stop(gstepper_t *stepper);

void gstepper_pause(gstepper_t *stepper);

void gstepper_resume(gstepper_t *stepper);

uint32_t gstepper_get_period(gstepper_t *stepper);

void gstepper_calc_plan(gstepper_t *stepper);