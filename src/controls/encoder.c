#include "controls/encoder.h"

static int32_t get_encoder_counter() {
    return ((int32_t) __HAL_TIM_GET_COUNTER(&htim2)) / 4;
}

gencoder_t encoder = {
    .init = {
        .counter = get_encoder_counter
    }
};