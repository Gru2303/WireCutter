#include "stepper.h"

void gstepper_init(gstepper_t *stepper) {
    if (!stepper) {
        return;
    }

    stepper->_internal.init = true;
    stepper->_internal.steps = -1;

    gstepper_disable(stepper);
    gstepper_dir(stepper, GSTEPPER_DIR_RIGHT);
}

void gstepper_deinit(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    stepper->_internal.init = false;
}

void gstepper_step(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    stepper->_internal.steps += stepper->_internal.dir;

    if (stepper->init.step) {
        stepper->init.step();
    }
}

void gstepper_dir(gstepper_t *stepper, gstepper_dir_t dir) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    stepper->_internal.dir = dir;

    if (stepper->init.dir) {
        stepper->init.dir(dir);
    }
}

void gstepper_enable(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    stepper->_internal.enabled = true;

    if (stepper->init.enable) {
        stepper->init.enable();
    }
}

void gstepper_disable(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    stepper->_internal.enabled = false;

    if (stepper->init.disable) {
        stepper->init.disable();
    }
}
