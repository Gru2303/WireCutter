#include "stepper.h"

void gstepper_init(gstepper_t *stepper) {
    if (!stepper) {
        return;
    }

    stepper->_internal.init = true;

    stepper->_internal.period = 10000;
    stepper->_internal.period_min = 10000;

    gstepper_enable(stepper);
    gstepper_set_dir(stepper, GSTEPPER_DIR_NONE);
    gstepper_set_max_speed(stepper, 100);
    gstepper_set_acceleration(stepper, 10000);
}

void gstepper_deinit(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    stepper->_internal.init = false;
}

bool gstepper_is_init(gstepper_t *stepper) {
    if (!stepper) {
        return false;
    }

    return stepper->_internal.init;
}

bool gstepper_can_move_in_dir(gstepper_t *stepper, gstepper_dir_t dir) {
    if (!stepper || !stepper->_internal.init) {
        return false;
    }

    if (stepper->_internal.dir == GSTEPPER_DIR_NONE) {
        return false;
    }

    if (!stepper->_internal.enabled) {
        return false;
    }

    if (stepper->init.can_move && !stepper->init.can_move(dir)) {
        return false;
    }

    if (stepper->_internal.has_limits) {
        int32_t next_pos = stepper->_internal.pos += stepper->_internal.dir;

        if (next_pos < stepper->_internal.min_pos || next_pos > stepper->_internal.max_pos) {
            return false;
        }
    }

    return true;
}

bool gstepper_can_move(gstepper_t *stepper) {
    return gstepper_can_move_in_dir(stepper, stepper->_internal.dir);
}

void gstepper_step(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    if (!gstepper_can_move(stepper)) {
        return;
    }

    stepper->_internal.pos += stepper->_internal.dir;

    if (stepper->init.step) {
        stepper->init.step();
    }
}

void gstepper_set_dir(gstepper_t *stepper, gstepper_dir_t dir) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    stepper->_internal.dir = dir;

    if (stepper->init.dir) {
        stepper->init.dir(dir);
    }
}

gstepper_dir_t gstepper_get_dir(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return GSTEPPER_DIR_NONE;
    }

    return stepper->_internal.dir;
}

gstepper_dir_t gstepper_toggle_dir(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return GSTEPPER_DIR_NONE;
    }

    if (stepper->_internal.dir == GSTEPPER_DIR_RIGHT) {
        gstepper_set_dir(stepper, GSTEPPER_DIR_LEFT);
    } else if (stepper->_internal.dir == GSTEPPER_DIR_LEFT) {
        gstepper_set_dir(stepper, GSTEPPER_DIR_RIGHT);
    }

    return stepper->_internal.dir;
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

bool gstepper_is_enabled(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return false;
    }

    return stepper->_internal.enabled;
}

void gstepper_enable_auto_power(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    stepper->_internal.auto_power = true;
}

void gstepper_disable_auto_power(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    stepper->_internal.auto_power = false;
}

bool gstepper_is_auto_power_enabled(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return false;
    }

    return stepper->_internal.auto_power;
}

void gstepper_set_pos(gstepper_t *stepper, int32_t pos) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    stepper->_internal.pos = pos;
}

int32_t gstepper_get_pos(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return 0;
    }

    return stepper->_internal.pos;
}

void gstepper_reset_pos(gstepper_t *stepper) {
    gstepper_set_pos(stepper, 0);
}

void gstepper_set_limits(gstepper_t *stepper, int32_t min, int32_t max) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    stepper->_internal.min_pos = min;
    stepper->_internal.max_pos = max;
    stepper->_internal.has_limits = true;
}

void gstepper_disable_limits(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    stepper->_internal.has_limits = false;
}

bool gstepper_has_limits(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return false;
    }

    return stepper->_internal.has_limits;
}

gstepper_state_t gstepper_tick(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return GSTEPPER_STATE_STOP;
    }

    if (stepper->_internal.state != GSTEPPER_STATE_STOP) {
        uint32_t thisUs = gstepper_get_micros();

        if (thisUs - stepper->_internal.timer >= stepper->_internal.period) {
            stepper->_internal.timer = thisUs;

            return gstepper_tick_manual(stepper);
        }
    }

    return GSTEPPER_STATE_STOP;
}

gstepper_state_t gstepper_tick_manual(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return GSTEPPER_STATE_STOP;
    }

    if (stepper->_internal.state == GSTEPPER_STATE_STOP) {
        return GSTEPPER_STATE_STOP;
    }

    if (!gstepper_can_move(stepper)) {
        gstepper_brake(stepper);

        return stepper->_internal.state;
    }

    gstepper_step(stepper);

    if (stepper->_internal.state == GSTEPPER_STATE_TARGET || stepper->_internal.state == GSTEPPER_STATE_PAUSE) {
        stepper->_internal.steps++;

        if (stepper->_internal.steps < stepper->_internal.accel_end_step) {
            if ((stepper->_internal.steps + stepper->_internal.start_step_offset) >= stepper->_internal.fast_profile_steps[GSTEPPER_FAST_PROFILE - 1]) {
                stepper->_internal.period = stepper->_internal.period_min;
            }else {
                int j = 0;

                while ((stepper->_internal.steps + stepper->_internal.start_step_offset) >= stepper->_internal.fast_profile_steps[j]) {
                    j++;
                }

                stepper->_internal.period = stepper->_internal.fast_profile_periods[j];
            }
        } else if (stepper->_internal.steps < stepper->_internal.decel_start_step) {
            stepper->_internal.period = stepper->_internal.period_min;
        } else if (stepper->_internal.steps < stepper->_internal.total_move_steps) {
            if ((stepper->_internal.total_move_steps - stepper->_internal.steps) >= stepper->_internal.fast_profile_steps[GSTEPPER_FAST_PROFILE - 1]) {
                stepper->_internal.period = stepper->_internal.period_min;
            } else {
                int j = 0;

                while ((stepper->_internal.total_move_steps - stepper->_internal.steps) >= stepper->_internal.fast_profile_steps[j]) {
                    j++;
                }

                stepper->_internal.period = stepper->_internal.fast_profile_periods[j];
            }
        } else {
            if (stepper->_internal.is_reversing) {
                gstepper_brake(stepper);
                gstepper_set_target(stepper, stepper->_internal.buffered_target_pos, false);

                return stepper->_internal.state;
            }

            if (stepper->_internal.state == GSTEPPER_STATE_TARGET) {
                stepper->_internal.is_target_reached = 1;
            }

            gstepper_brake(stepper);
        }

        return stepper->_internal.state;
    }

    if (stepper->_internal.state == GSTEPPER_STATE_SMOOTH_STOP) {
        stepper->_internal.smooth_stop_steps--;

        if (stepper->_internal.smooth_stop_steps >= stepper->_internal.fast_profile_steps[GSTEPPER_FAST_PROFILE - 1]) {
            stepper->_internal.period = stepper->_internal.period_min;
        } else {
            int j = 0;

            while (stepper->_internal.smooth_stop_steps >= stepper->_internal.fast_profile_steps[j]) {
                j++;
            }

            stepper->_internal.period = stepper->_internal.fast_profile_periods[j];
        }

        if (stepper->_internal.pos == stepper->_internal.target_pos || stepper->_internal.smooth_stop_steps <= 0 || stepper->_internal.period >= stepper->_internal.start_period) {
            gstepper_brake(stepper);
        }

        return stepper->_internal.state;
    }

    return stepper->_internal.state;
}

gstepper_state_t gstepper_get_state(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return GSTEPPER_STATE_STOP;
    }

    return stepper->_internal.state;
}

bool gstepper_ready(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return false;
    }

    if (stepper->_internal.state == GSTEPPER_STATE_STOP && stepper->_internal.is_target_reached) {
        stepper->_internal.is_target_reached = 0;

        return true;
    }

    return false;
}

void gstepper_set_target(gstepper_t *stepper, int32_t target, gstepper_target_mode_t relative) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    if (stepper->_internal.is_speed_zero) {
        gstepper_brake(stepper);

        stepper->_internal.is_target_reached = 1;

        return;
    }

    if (stepper->_internal.settings_changed) {
        stepper->_internal.period_min = stepper->_internal.pending_min_period;
        stepper->_internal.max_speed = 1000000L / stepper->_internal.period_min;

        gstepper_set_acceleration(stepper, stepper->_internal.pending_acceleration);

        stepper->_internal.settings_changed = 0;
    }

    if (relative == GSTEPPER_TARGET_MODE_RELATIVE) {
        stepper->_internal.target_pos = target + stepper->_internal.pos;
    } else {
        stepper->_internal.target_pos = target;
    }

    if (stepper->_internal.target_pos == stepper->_internal.pos) {
        gstepper_brake(stepper);

        stepper->_internal.is_target_reached = 1;

        return;
    }

    stepper->_internal.is_reversing = 0;
    stepper->_internal.total_move_steps = abs(stepper->_internal.target_pos - stepper->_internal.pos);

    gstepper_dir_t ndir = stepper->_internal.pos < stepper->_internal.target_pos ? GSTEPPER_DIR_RIGHT : GSTEPPER_DIR_LEFT;
    int32_t v1 = 0;

    if (stepper->_internal.state != GSTEPPER_STATE_STOP) {
        v1 = (int32_t) (1000000L / stepper->_internal.period);
    }

    int32_t ss = 0;

    if (stepper->_internal.acceleration > 0) {
        ss = v1 * v1 / (2L * stepper->_internal.acceleration);
    }

    if (ss > stepper->_internal.total_move_steps || (stepper->_internal.state != GSTEPPER_STATE_STOP && ndir != stepper->_internal.dir)) {
        stepper->_internal.is_reversing = 1;
        stepper->_internal.buffered_target_pos = stepper->_internal.target_pos;
        stepper->_internal.target_pos = stepper->_internal.pos + ss * stepper->_internal.dir;
        stepper->_internal.total_move_steps = ss;
    }

    if (stepper->_internal.acceleration > 0 && stepper->_internal.period_min < 300000) {
        if ((int32_t)stepper->_internal.max_speed * stepper->_internal.max_speed / stepper->_internal.acceleration - (v1 * v1 / stepper->_internal.acceleration >> 1) > stepper->_internal.total_move_steps) {
            if (stepper->_internal.is_reversing) {
                stepper->_internal.accel_end_step = 0;
            } else {
                stepper->_internal.accel_end_step = (stepper->_internal.total_move_steps >> 1) - (v1 * v1 / stepper->_internal.acceleration >> 2);
            }

            stepper->_internal.decel_start_step = stepper->_internal.accel_end_step;
        } else {
            stepper->_internal.accel_end_step = (stepper->_internal.max_speed * stepper->_internal.max_speed - v1 * v1) / (2L * stepper->_internal.acceleration);
            stepper->_internal.decel_start_step = stepper->_internal.total_move_steps - stepper->_internal.max_speed * stepper->_internal.max_speed / (2L * stepper->_internal.acceleration);
        }

        stepper->_internal.start_step_offset = v1 * v1 / (2L * stepper->_internal.acceleration);

        if (v1 == 0) {
            stepper->_internal.period = stepper->_internal.start_period;
        }
    } else {
        stepper->_internal.accel_end_step = stepper->_internal.start_step_offset = 0;
        stepper->_internal.decel_start_step = stepper->_internal.total_move_steps;
        stepper->_internal.period = stepper->_internal.period_min;
    }

    stepper->_internal.period_scaled = stepper->_internal.period << 10;
    stepper->_internal.steps = 0;
    stepper->_internal.state = GSTEPPER_STATE_TARGET;

    gstepper_set_dir(stepper, ndir);

    if (stepper->_internal.auto_power) {
        gstepper_enable(stepper);
    }

    stepper->_internal.is_target_reached = 0;
}

void gstepper_set_target_with_speed(gstepper_t *stepper, int32_t target, gstepper_target_mode_t relative, int32_t speed) {
    gstepper_set_max_speed(stepper, speed);
    gstepper_set_target(stepper, target, relative);
}

int32_t gstepper_get_target(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return 0;
    }

    return stepper->_internal.is_reversing ? stepper->_internal.buffered_target_pos : stepper->_internal.target_pos;
}

bool gstepper_set_speed(gstepper_t *stepper, int32_t speed) {
    if (!stepper || !stepper->_internal.init) {
        return false;
    }

    if (speed == 0) {
        gstepper_brake(stepper);

        return false;
    }

    gstepper_set_dir(stepper, speed > 0 ? GSTEPPER_DIR_RIGHT : GSTEPPER_DIR_LEFT);

    stepper->_internal.period = 1000000L / abs(speed);
    stepper->_internal.state = GSTEPPER_STATE_SPEED;

    if (stepper->_internal.auto_power) {
        gstepper_enable(stepper);
    }

    return true;
}

void gstepper_set_max_speed(gstepper_t *stepper, int32_t max_speed) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    if (max_speed == 0) {
        stepper->_internal.is_speed_zero = 1;

        return;
    }

    stepper->_internal.is_speed_zero = 0;
    stepper->_internal.pending_min_period = 1000000 / max_speed;

    if (stepper->_internal.state == GSTEPPER_STATE_STOP) {
        stepper->_internal.period_min = stepper->_internal.pending_min_period;
        stepper->_internal.max_speed = (uint16_t)max_speed;

        gstepper_set_acceleration(stepper, stepper->_internal.acceleration);

        stepper->_internal.settings_changed = 0;
    } else {
        stepper->_internal.settings_changed = 1;
    }
}

void gstepper_set_acceleration(gstepper_t *stepper, int32_t accel) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    stepper->_internal.pending_acceleration = accel;

    if (stepper->_internal.state == GSTEPPER_STATE_STOP) {
        stepper->_internal.acceleration = stepper->_internal.pending_acceleration;

        if (stepper->_internal.acceleration > 0) {
            stepper->_internal.start_period = (uint32_t) (0.676 * 1000000.0 * sqrt(2.0 / stepper->_internal.acceleration) + 0.5);
        } else {
            stepper->_internal.start_period = stepper->_internal.period_min;
        }

        stepper->_internal.settings_changed = 0;

        gstepper_calc_plan(stepper);
    } else {
        stepper->_internal.settings_changed = 1;
    }
}

void gstepper_brake(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    stepper->_internal.state = GSTEPPER_STATE_STOP;

    if (stepper->_internal.auto_power) {
        gstepper_disable(stepper);
    }
}

void gstepper_stop(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    if (stepper->_internal.acceleration == 0 || stepper->_internal.period > 300000 || stepper->_internal.state == GSTEPPER_STATE_SPEED || stepper->_internal.state == GSTEPPER_STATE_STOP) {
        gstepper_brake(stepper);

        return;
    }

    if (stepper->_internal.state == GSTEPPER_STATE_TARGET || stepper->_internal.state == GSTEPPER_STATE_PAUSE) {
        if (stepper->_internal.steps > stepper->_internal.decel_start_step) {
            gstepper_pause(stepper);

            return;
        }

        stepper->_internal.smooth_stop_steps = (int16_t) (1000000ul / stepper->_internal.period);
        stepper->_internal.smooth_stop_steps = (int16_t) (stepper->_internal.smooth_stop_steps * stepper->_internal.smooth_stop_steps / 2 / stepper->_internal.acceleration);
        stepper->_internal.period_scaled = stepper->_internal.period << 10;
        stepper->_internal.state = GSTEPPER_STATE_SMOOTH_STOP;
    }
}

void gstepper_pause(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    if (stepper->_internal.state == GSTEPPER_STATE_TARGET) {
        stepper->_internal.state = GSTEPPER_STATE_PAUSE;
    }
}

void gstepper_resume(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    if (stepper->_internal.state == GSTEPPER_STATE_STOP) {
        gstepper_set_target(stepper, stepper->_internal.target_pos, false);
    }
}

uint32_t gstepper_get_period(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return UINT32_MAX;
    }

    return stepper->_internal.period;
}

void gstepper_calc_plan(gstepper_t *stepper) {
    if (!stepper || !stepper->_internal.init) {
        return;
    }

    if (stepper->_internal.acceleration > 0) {
        const uint32_t sa = stepper->_internal.max_speed * stepper->_internal.max_speed / stepper->_internal.acceleration / 2ul;
        const double dtf = sqrt(2.0 * sa / stepper->_internal.acceleration) / GSTEPPER_FAST_PROFILE;
        const double s0 = stepper->_internal.acceleration * dtf * dtf / 2.0;
        const uint32_t dt = (uint32_t) (dtf * 1000000.0);

        for (int i = 0; i < GSTEPPER_FAST_PROFILE; i++) {
            stepper->_internal.fast_profile_steps[i] = (uint32_t) (s0 * (i + 1) * (i + 1));

            uint32_t ds = stepper->_internal.fast_profile_steps[i];

            if (i > 0) {
                ds -= stepper->_internal.fast_profile_steps[i - 1];
            }

            if (ds <= 0) {
                stepper->_internal.fast_profile_periods[i] = 0;
            } else {
                stepper->_internal.fast_profile_periods[i] = dt / ds;
            }
        }
    }
}
