#include "motors/cut_motor.h"

static void step() {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

    dwt_delay(1);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

static void dir(gstepper_dir_t dir) {
    if (dir == GSTEPPER_DIR_RIGHT) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    } else if (dir == GSTEPPER_DIR_LEFT) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    }
}

static bool can_move(gstepper_dir_t dir) {
    if (dir == GSTEPPER_DIR_LEFT) {
        return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_SET;
    }

    return true;
}

gstepper_t cut_motor = {
    .init = {
        .step = step,
        .dir = dir,
        .can_move = can_move
    }
};