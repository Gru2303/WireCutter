#include "menu/menu.h"

GCRYSLCD_MENU_INIT_PAGE(menu_about_page, &menu_main_page,
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_BUTTON, .textWidget = { .text = "    WireCutter", .noAvailable = &menu_always_true }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_TEXT, .textWidget = { .text = "Version: v1.0", .noAvailable = &menu_always_true }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_TEXT, .textWidget = { .text = "Author: Gru2303", .noAvailable = &menu_always_true }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_SUBMENU, .submenuWidget = { .base.text = "Back", .page = &menu_main_page }  },
);