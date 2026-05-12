#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    GSTEPPER_DIR_RIGHT = 1,
    GSTEPPER_DIR_LEFT = -1
} gstepper_dir_t;

typedef struct {
    void (*step)();
    void (*dir)(gstepper_dir_t dir);
    void (*enable)();
    void (*disable)();
} gstepper_init_t;

typedef struct {
    bool init;

    int32_t steps;
    gstepper_dir_t dir;
    bool enabled;
} gstepper_internal_t;

typedef struct {
    gstepper_init_t init;

    gstepper_internal_t _internal;
} gstepper_t;

extern uint32_t gstepper_get_micros();

void gstepper_init(gstepper_t *stepper);

void gstepper_deinit(gstepper_t *stepper);

void gstepper_step(gstepper_t *stepper);

void gstepper_dir(gstepper_t *stepper, gstepper_dir_t dir);

void gstepper_enable(gstepper_t *stepper);

void gstepper_disable(gstepper_t *stepper);