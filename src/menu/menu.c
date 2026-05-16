#include "menu/menu.h"

const char *vals[] = { "0.5mm", "1.0mm", "1.5mm"};

bool menu_always_true = true;
bool menu_always_false = false;

gcryslcd_menu_t menu = {
    .init = {
        .lcd = &display,
    }
};