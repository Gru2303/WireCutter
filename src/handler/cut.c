#include "handler/cut.h"

static handler_cutting_state_t state;
static handler_cutting_settings_t settings;
static uint32_t count_done;

static gcryslcd_menu_widget_t count_text_widget = { .type = GCRYSLCD_MENU_WIDGET_TEXT, .textWidget = { .text = "                    ", .noAvailable = &menu_always_true }  };
static gcryslcd_menu_widget_t count_done_text_widget = { .type = GCRYSLCD_MENU_WIDGET_TEXT, .textWidget = { .text = "                    ", .noAvailable = &menu_always_true }  };

static GCRYSLCD_MENU_INIT_PAGE(progress_page, &menu_main_page,
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_TEXT, .textWidget = { .text = "Progress:", .noAvailable = &menu_always_true }  },
    &count_text_widget,
    &count_done_text_widget,
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_BUTTON, .buttonWidget = { .base.text = "STOP", .action = handler_cutting_stop }  },
);

void handler_cutting_start(handler_cutting_settings_t new_settings) {
    if (state != HANDLER_CUTTING_STATE_IDLE) {
        return;
    }

    settings = new_settings;

    gstepper_set_max_speed(&wire_motor, menu_wire_motor_max_speed);
    gstepper_set_acceleration(&wire_motor, menu_wire_motor_accel);
    gstepper_set_acceleration(&cut_motor, menu_cut_motor_accel);

    if (!handler_is_homed()) {
        handler_home();

        state = HANDLER_CUTTING_STATE_HOMING;
    } else {
        gstepper_set_target_with_speed(&cut_motor, settings.start_cut_pos, GSTEPPER_TARGET_MODE_ABSOLUTE, menu_cut_motor_max_speed);
        state = HANDLER_CUTTING_STATE_PREPARATION;
    }

    gcryslcd_menu_set_page(&menu, &progress_page);
}

void handler_cutting_tick() {
    snprintf(count_text_widget.baseWidget.text, 20, "All: %lu", settings.count);
    snprintf(count_done_text_widget.baseWidget.text, 20, "Done: %lu", count_done);

    switch (state) {
        case HANDLER_CUTTING_STATE_IDLE: {
            break;
        }
        case HANDLER_CUTTING_STATE_HOMING: {
            if (handler_is_homed()) {
                gstepper_set_target_with_speed(&cut_motor, settings.start_cut_pos, GSTEPPER_TARGET_MODE_ABSOLUTE, menu_cut_motor_max_speed);
                state = HANDLER_CUTTING_STATE_PREPARATION;
            }

            break;
        }
        case HANDLER_CUTTING_STATE_PREPARATION: {
            if (gstepper_ready(&cut_motor)) {
                gstepper_set_target(&wire_motor, menu_wire_motor_step_per_mm * settings.wire_strip_start, GSTEPPER_TARGET_MODE_RELATIVE);
                state = HANDLER_CUTTING_STATE_FEED_STRIP_1;
            }

            break;
        }
        case HANDLER_CUTTING_STATE_FEED_STRIP_1: {
            if (gstepper_ready(&wire_motor)) {
                gstepper_set_target_with_speed(&cut_motor, settings.partial_cut_pos, GSTEPPER_TARGET_MODE_ABSOLUTE, menu_cut_motor_max_cut_speed);
                state = HANDLER_CUTTING_STATE_PARTIAL_CUT_1;
            }

            break;
        }
        case HANDLER_CUTTING_STATE_PARTIAL_CUT_1: {
            if (gstepper_ready(&cut_motor)) {
                gstepper_set_target_with_speed(&cut_motor, settings.start_cut_pos, GSTEPPER_TARGET_MODE_ABSOLUTE, menu_cut_motor_max_speed);
                state = HANDLER_CUTTING_STATE_RAISE_BLADE_1;
            }

            break;
        }
        case HANDLER_CUTTING_STATE_RAISE_BLADE_1: {
            if (gstepper_ready(&cut_motor)) {
                gstepper_set_target(&wire_motor, menu_wire_motor_step_per_mm * settings.wire_length, GSTEPPER_TARGET_MODE_RELATIVE);
                state = HANDLER_CUTTING_STATE_FEED_MAIN;
            }

            break;
        }
        case HANDLER_CUTTING_STATE_FEED_MAIN: {
            if (gstepper_ready(&wire_motor)) {
                gstepper_set_target_with_speed(&cut_motor, settings.partial_cut_pos, GSTEPPER_TARGET_MODE_ABSOLUTE, menu_cut_motor_max_cut_speed);
                state = HANDLER_CUTTING_STATE_PARTIAL_CUT_2;
            }

            break;
        }
        case HANDLER_CUTTING_STATE_PARTIAL_CUT_2: {
            if (gstepper_ready(&cut_motor)) {
                gstepper_set_target_with_speed(&cut_motor, settings.start_cut_pos, GSTEPPER_TARGET_MODE_ABSOLUTE, menu_cut_motor_max_speed);
                state = HANDLER_CUTTING_STATE_RAISE_BLADE_2;
            }

            break;
        }
        case HANDLER_CUTTING_STATE_RAISE_BLADE_2: {
            if (gstepper_ready(&cut_motor)) {
                gstepper_set_target(&wire_motor, menu_wire_motor_step_per_mm * settings.wire_strip_end, GSTEPPER_TARGET_MODE_RELATIVE);
                state = HANDLER_CUTTING_STATE_FEED_STRIP_2;
            }

            break;
        }
        case HANDLER_CUTTING_STATE_FEED_STRIP_2: {
            if (gstepper_ready(&wire_motor)) {
                gstepper_set_target_with_speed(&cut_motor, settings.full_cut_pos, GSTEPPER_TARGET_MODE_ABSOLUTE, menu_cut_motor_max_cut_speed);
                state = HANDLER_CUTTING_STATE_FULL_CUT;
            }

            break;
        }
        case HANDLER_CUTTING_STATE_FULL_CUT: {
            if (gstepper_ready(&cut_motor)) {
                gstepper_set_target_with_speed(&cut_motor, settings.start_cut_pos, GSTEPPER_TARGET_MODE_ABSOLUTE, menu_cut_motor_max_speed);
                state = HANDLER_CUTTING_STATE_RAISE_BLADE_3;
            }

            break;
        }
        case HANDLER_CUTTING_STATE_RAISE_BLADE_3: {
            if (gstepper_ready(&cut_motor)) {
                gstepper_set_target_with_speed(&cut_motor, settings.start_cut_pos, GSTEPPER_TARGET_MODE_ABSOLUTE, menu_cut_motor_max_speed);
                state = HANDLER_CUTTING_STATE_CHECK_COUNT;
            }

            break;
        }
        case HANDLER_CUTTING_STATE_CHECK_COUNT: {
            if (settings.count < count_done) {
                count_done++;
                state = HANDLER_CUTTING_STATE_PREPARATION;
            } else {
                state = HANDLER_CUTTING_STATE_IDLE;

                gcryslcd_menu_set_page(&menu, settings.end_page);
            }

            break;
        }
        case HANDLER_CUTTING_STATE_STOP: {
            gstepper_set_target(&cut_motor, settings.start_cut_pos, GSTEPPER_TARGET_MODE_ABSOLUTE);
            gstepper_brake(&wire_motor);
            state = HANDLER_CUTTING_STATE_IDLE;

            gcryslcd_menu_set_page(&menu, settings.end_page);

            break;
        }
    }
}

bool handler_cutting_is_running() {
    return state != HANDLER_CUTTING_STATE_IDLE;
}

void handler_cutting_stop() {
    state = HANDLER_CUTTING_STATE_STOP;
}

uint32_t handler_cutting_get_count() {
    if (!handler_cutting_is_running()) {
        return 0;
    }

    return settings.count;
}

uint32_t handler_cutting_get_count_done() {
    if (!handler_cutting_is_running()) {
        return 0;
    }

    return count_done;
}