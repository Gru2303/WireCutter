#include "controls/buttons.h"

uint32_t gbutton_get_millis(void) {
    return HAL_GetTick();
}

static bool _read_control_button() {
    return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET;
}

gbutton_t control_button = {
    .init = {
        .gpio = {
            .read = _read_control_button
        },
        .time = {
            .hold = 600,
            .step = 200,
            .click = 500,
            .deb = 50
        },
        .callback = {0}
    }
};