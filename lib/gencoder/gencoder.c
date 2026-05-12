#include "gencoder.h"

void gencoder_tick(gencoder_t *encoder) {
    if (!encoder) {
        return;
    }

    int32_t counter = encoder->init.counter();
    int32_t last = encoder->_internal.last_counter;

    encoder->_internal.last_counter = counter;

    encoder->_internal.steps = counter - last;
}

bool gencoder_turn(gencoder_t *encoder) {
    if (!encoder) {
        return false;
    }

    return encoder->_internal.steps != 0;
}

int8_t gencoder_dir(gencoder_t *encoder) {
    if (!encoder) {
        return 0;
    }

    int32_t steps = encoder->_internal.steps;

    if (steps > 0) {
        return 1;
    }

    if (steps < 0) {
        return -1;
    }

    return 0;
}

int32_t gencoder_steps(gencoder_t *encoder) {
    if (!encoder) {
        return 0;
    }

    return encoder->_internal.steps;
}
