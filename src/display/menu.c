#include <display/menu.h>

GCRYSLCD_MENU_DEFINE_PAGE(main_page);
GCRYSLCD_MENU_DEFINE_PAGE(about_page);

GCRYSLCD_MENU_INIT_PAGE(about_page, &main_page,
    { .type = GCRYSLCD_MENU_WIDGET_BUTTON, .textWidget = { .text = "    WireCutter", .noAvailable = true }  },
    { .type = GCRYSLCD_MENU_WIDGET_TEXT, .textWidget = { .text = "Version: v1.0", .noAvailable = true }  },
    { .type = GCRYSLCD_MENU_WIDGET_TEXT, .textWidget = { .text = "Author: Gru2303", .noAvailable = true }  },
    { .type = GCRYSLCD_MENU_WIDGET_SUBMENU, .submenuWidget = { .base.text = "Back", .page = &main_page }  },
);

GCRYSLCD_MENU_INIT_PAGE(main_page, NULL,
    { .type = GCRYSLCD_MENU_WIDGET_SUBMENU, .submenuWidget = { .base.text = "    WireCutter", .page = &about_page }  },
    { .type = GCRYSLCD_MENU_WIDGET_TEXT, .textWidget = { .text = "", .noAvailable = true }  },
    { .type = GCRYSLCD_MENU_WIDGET_BUTTON, .buttonWidget = { .base.text = "Start", .action = NULL }  },
    { .type = GCRYSLCD_MENU_WIDGET_BUTTON, .buttonWidget = { .base.text = "Home", .action = NULL }  },
    { .type = GCRYSLCD_MENU_WIDGET_SUBMENU, .submenuWidget = { .base.text = "Presets", .page = NULL }  },
    { .type = GCRYSLCD_MENU_WIDGET_SUBMENU, .submenuWidget = { .base.text = "Settings", .page = NULL }  },
);

gcryslcd_menu_t menu = {
    .init = {
        .lcd = &display,
    }
};