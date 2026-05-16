#include <peripherals/dwt.h>

void dwt_init() {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void dwt_deinit() {
    CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk;

    DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
    DWT->CYCCNT = 0;
}

uint32_t dwt_get_tick() {
    return DWT->CYCCNT;
}

uint32_t dwt_get_micros() {
    return DWT->CYCCNT / (SystemCoreClock / 1000000U);
}

void dwt_delay(uint32_t micros) {
    uint32_t start = dwt_get_micros();
    uint32_t wait = micros;

    if (wait < 0xFFFFFFFFU) {
        wait += 1;
    }

    while ((dwt_get_micros() - start) < wait) {
    }
}
