#include "main.h"

static void controls_tick();

int main() {
    HAL_Init();

    system_clock_config();
    gpio_init();
    timer_init();
    dwt_init();
    i2c_init();

    watchdog_init();

    setup();

    while (1) {
        watchdog_tick();

        loop();
    }
}

void setup() {
    gcryslcd_menu_init(&menu, &menu_main_page);
    gstepper_init(&wire_motor);
    gstepper_init(&cut_motor);

    HAL_Delay(100);
}

void loop() {
    controls_tick();
    handler_home_tick();
    handler_cutting_tick();
}

void controls_tick() {
    gbutton_tick(&control_button);
    gencoder_tick(&encoder);

    if (gbutton_click_with_clicks(&control_button, 1)) {
        gcryslcd_menu_handle_input(&menu, GCRYSLCD_MENU_INPUT_ENTER);
    } else if (gbutton_hold(&control_button)) {
        gcryslcd_menu_handle_input(&menu, GCRYSLCD_MENU_INPUT_BACK);
    }

    if (gencoder_turn(&encoder)) {
        if (gencoder_dir(&encoder) > 0) {
            gcryslcd_menu_handle_input(&menu, GCRYSLCD_MENU_INPUT_DOWN);
        } else if (gencoder_dir(&encoder) < 0) {
            gcryslcd_menu_handle_input(&menu, GCRYSLCD_MENU_INPUT_UP);
        }
    }

    gcryslcd_menu_render(&menu);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM1) {
        gstepper_tick(&wire_motor);
        gstepper_tick(&cut_motor);
    }
}




