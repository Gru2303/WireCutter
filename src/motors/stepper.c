#include <gstepper/gstepper.h>
#include "peripherals/dwt.h"

uint32_t gstepper_get_micros() {
    return dwt_get_micros();
}