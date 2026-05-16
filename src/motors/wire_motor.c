#include "motors/wire_motor.h"

static void step() {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);

    dwt_delay(1);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
}

static void dir(gstepper_dir_t dir) {
    if (dir == GSTEPPER_DIR_RIGHT) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
    } else if (dir == GSTEPPER_DIR_LEFT) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
    }
}

gstepper_t wire_motor = {
    .init = {
        .step = step,
        .dir = dir
    }
};