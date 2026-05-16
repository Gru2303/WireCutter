#include "menu/menu.h"

#include "handler/cut.h"

static const char *values[] = { "0.5mm", "1.0mm", "1.5mm", "2.0mm", "2.5mm", "3.0mm", "3.5mm", "4.0mm", "4.5mm", "5.0mm" };
static const menu_profile_t *profiles[] = { &menu_profile_0_5, &menu_profile_1_0, &menu_profile_1_5, &menu_profile_2_0, &menu_profile_2_5, &menu_profile_3_0, &menu_profile_3_5, &menu_profile_4_0, &menu_profile_4_5, &menu_profile_5_0 };
static uint32_t selected_index = 0;
static int32_t wire_strip_start = 10;
static int32_t wire_length = 50;
static int32_t wire_strip_end = 10;
static int32_t count = 1;

static void action_start() {
    handler_cutting_settings_t settings = {
        .wire_strip_start = wire_strip_start,
        .wire_length = wire_length,
        .wire_strip_end = wire_strip_end,
        .start_cut_pos = profiles[selected_index]->start_cut_pos,
        .partial_cut_pos = profiles[selected_index]->partial_cut_pos,
        .full_cut_pos = profiles[selected_index]->full_cut_pos,
        .count = count,

        .end_page = &menu_main_page
    };

    handler_cutting_start(settings);
}

GCRYSLCD_MENU_INIT_PAGE(menu_start_page, &menu_main_page,
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_SELECT, .selectWidget = { .base.text = "Profile", .values_count = 10, .values = values, .selected_index = &selected_index }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_INT, .intWidget = { .base.text = "Start len", .min = 1, .max = 1000, .step = 1, .value = &wire_strip_start }, },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_INT, .intWidget = { .base.text = "Len", .min = 1, .max = 1000, .step = 1, .value = &wire_length }, },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_INT, .intWidget = { .base.text = "End len", .min = 1, .max = 1000, .step = 1, .value = &wire_strip_end }, },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_INT, .intWidget = { .base.text = "Count", .min = 1, .max = 1000, .step = 1, .value = &count }, },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_BUTTON, .buttonWidget = { .base.text = "Start", .action = action_start }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_SUBMENU, .submenuWidget = { .base.text = "Back", .page = &menu_main_page }  },
);