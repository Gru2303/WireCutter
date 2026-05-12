#include <peripherals/watchdog.h>

IWDG_HandleTypeDef hiwdg;

void watchdog_init() {
    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
    hiwdg.Init.Reload = 4095;

    if (HAL_IWDG_Init(&hiwdg) != HAL_OK) {
        Error_Handler();
    }
}

void watchdog_tick() {
    HAL_IWDG_Refresh(&hiwdg);
}
