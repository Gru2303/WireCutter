#include "display/display.h"

uint32_t gcryslcd_get_millis() {
    return HAL_GetTick();
}

void gcryslcd_delay(uint32_t ms) {
    HAL_Delay(ms);
}

static void display_send_cmd(uint8_t data) {
    uint8_t data_t[4];

    char data_h = data & 0xf0;
    char data_l = (data << 4) & 0xf0;

    data_t[0] = data_h | DISPLAY_EN  | DISPLAY_RS_COMMAND;
    data_t[1] = data_h | DISPLAY_RS_COMMAND;

    data_t[2] = data_l | DISPLAY_EN  | DISPLAY_RS_COMMAND;
    data_t[3] = data_l | DISPLAY_RS_COMMAND;

    HAL_I2C_Master_Transmit(&hi2c1, (DISPLAY_ADDR << 1), data_t, 4, 100);
}

static void display_send_data(uint8_t data) {
    uint8_t data_t[4];

    char data_h = data & 0xf0;
    char data_l = (data << 4) & 0xf0;

    data_t[0] = data_h | DISPLAY_EN  | DISPLAY_RS_DATA;
    data_t[1] = data_h | DISPLAY_RS_DATA;

    data_t[2] = data_l | DISPLAY_EN  | DISPLAY_RS_DATA;
    data_t[3] = data_l | DISPLAY_RS_DATA;

    HAL_I2C_Master_Transmit(&hi2c1, (DISPLAY_ADDR << 1), data_t, 4, 100);
}

gcryslcd_t display = {
    .init = {
        .hw = {
            .send_cmd = display_send_cmd,
            .send_data = display_send_data
        },
        .mode = GCRYSLCD_4BIT_MODE,
        .cols = 20,
        .lines = 4,
    }
};