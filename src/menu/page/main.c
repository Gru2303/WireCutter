#include "menu/menu.h"

GCRYSLCD_MENU_INIT_PAGE(menu_main_page, NULL,
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_SUBMENU, .submenuWidget = { .base.text = "    WireCutter", .page = &menu_about_page }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_TEXT, .textWidget = { .text = "", .noAvailable = &menu_always_true }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_SUBMENU, .submenuWidget = { .base.text = "Start", .page = &menu_start_page }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_BUTTON, .buttonWidget = { .base.text = "Home", .action = handler_home }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_SUBMENU, .submenuWidget = { .base.text = "Presets", .page = &menu_pressets_page }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_SUBMENU, .submenuWidget = { .base.text = "Settings", .page = &menu_settings_page }  },
);