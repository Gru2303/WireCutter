#include "../../include/handler/home.h"

#include "motors/motors.h"

static handler_home_state_t state = HANDLER_HOME_IDLE;

void handler_home() {
    if (handler_is_homed()) {
        return;
    }

    gstepper_set_speed(&cut_motor, -200);
    state = HANDLER_HOMING_FAST_APPROACH;
}

void handler_home_tick() {
    switch (state) {
        case HANDLER_HOME_IDLE:
        case HANDLER_HOMING_DONE:
            break;

        case HANDLER_HOMING_FAST_APPROACH:
            if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET) {
                gstepper_set_target(&cut_motor, 200, GSTEPPER_TARGET_MODE_RELATIVE);

                state = HANDLER_HOMING_BACKOFF;
            }

            break;

        case HANDLER_HOMING_BACKOFF:
            if (gstepper_ready(&cut_motor)) {
                gstepper_set_speed(&cut_motor, -50);

                state = HANDLER_HOMING_SLOW_APPROACH;
            }

            break;

        case HANDLER_HOMING_SLOW_APPROACH:
            if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET) {
                gstepper_brake(&cut_motor);
                gstepper_reset_pos(&cut_motor);

                state = HANDLER_HOMING_DONE;
            }

            break;
    }
}

void handler_reset_home() {
    state = HANDLER_HOME_IDLE;
}

handler_home_state_t handler_home_get_state() {
    return state;
}

bool handler_is_homed() {
    return state == HANDLER_HOMING_DONE;
}