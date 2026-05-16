#include "menu/menu.h"

int32_t menu_wire_motor_max_speed = 5000;
int32_t menu_wire_motor_accel = 10000;
int32_t menu_wire_motor_step_per_mm = 25;

int32_t menu_cut_motor_max_speed = 1000;
int32_t menu_cut_motor_max_cut_speed = 1000;
int32_t menu_cut_motor_accel = 10000;

GCRYSLCD_MENU_INIT_PAGE(menu_settings_page, &menu_main_page,
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_BUTTON, .buttonWidget = { .base.text = "Home", .action = handler_home }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_SUBMENU, .submenuWidget = { .base.text = "Wire Motor", .page = &menu_settings_wire_motor_page }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_SUBMENU, .submenuWidget = { .base.text = "Cut Motor", .page = &menu_settings_cut_motor_page }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_SUBMENU, .submenuWidget = { .base.text = "Back", .page = &menu_main_page }  },
);

GCRYSLCD_MENU_INIT_PAGE(menu_settings_wire_motor_page, &menu_settings_page,
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_INT, .intWidget = { .base.text = "Speed", .min = 1, .max = 100000, .step = 10, .value = &menu_wire_motor_max_speed }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_INT, .intWidget = { .base.text = "Accel", .min = 1, .max = 100000, .step = 10, .value = &menu_wire_motor_accel }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_INT, .intWidget = { .base.text = "Step/mm", .min = 1, .max = 100000, .step = 1, .value = &menu_wire_motor_step_per_mm }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_SUBMENU, .submenuWidget = { .base.text = "Back", .page = &menu_settings_page }  },
);

GCRYSLCD_MENU_INIT_PAGE(menu_settings_cut_motor_page, &menu_settings_page,
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_INT, .intWidget = { .base.text = "Speed", .min = 1, .max = 100000, .step = 10, .value = &menu_cut_motor_max_speed }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_INT, .intWidget = { .base.text = "Cut speed", .min = 1, .max = 100000, .step = 10, .value = &menu_cut_motor_max_cut_speed }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_INT, .intWidget = { .base.text = "Accel", .min = 1, .max = 100000, .step = 10, .value = &menu_cut_motor_accel }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_SUBMENU, .submenuWidget = { .base.text = "Back", .page = &menu_settings_page }  },
);