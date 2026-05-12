#include "main.h"

static void controls_tick();

int main() {
    HAL_Init();

    system_clock_config();
    gpio_init();
    dma_init();
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

static void dir(gstepper_dir_t dir) {
    // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    //
    // for (int j = 0; j < 50; j++) {
    //     __NOP();
    // }
}

static void gcryslcd_delaytest(uint32_t micros) {
    uint32_t start = dwt_get_micros();
    uint32_t wait = micros;

    if (wait < 0xFFFFFFFFU) {
        wait += 1;
    }

    while ((dwt_get_micros() - start) < wait) {
    }
}

static void step() {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

    gcryslcd_delaytest(1);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

    gcryslcd_delaytest(300);
}

gstepper_t stepper = {
    .init = {
        .dir = dir,
        .step = step,
    }
};

void setup() {
    gcryslcd_menu_init(&menu, &main_page);
    gstepper_init(&stepper);

    HAL_Delay(100);
}

void loop() {
    controls_tick();

    gstepper_step(&stepper);
}

void controls_tick() {
    gbutton_tick(&control_button);
    gencoder_tick(&encoder);

    if (gbutton_click_with_clicks(&control_button, 1)) {
        gcryslcd_menu_handle_input(&menu, GCRYSLCD_MENU_INPUT_ENTER);
    } else if (gbutton_click_with_clicks(&control_button, 2)) {
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




