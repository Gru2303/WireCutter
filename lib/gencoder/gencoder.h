#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int32_t (*counter)();
} gencoder_init_t;

typedef struct {
    int32_t last_counter;
    int32_t steps;
} gencoder_internal_t;

typedef struct {
    gencoder_init_t init;

    gencoder_internal_t _internal;
} gencoder_t;

void gencoder_tick(gencoder_t *encoder);

bool gencoder_turn(gencoder_t *encoder);

int8_t gencoder_dir(gencoder_t *encoder);

int32_t gencoder_steps(gencoder_t *encoder);