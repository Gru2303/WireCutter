#include "menu/menu.h"

menu_profile_t menu_profile_0_5 = {
    .start_cut_pos = 800,
    .partial_cut_pos = 150,
    .full_cut_pos = 10,
};
menu_profile_t menu_profile_1_0 = {
    .start_cut_pos = 800,
    .partial_cut_pos = 150,
    .full_cut_pos = 10,
};
menu_profile_t menu_profile_1_5 = {
    .start_cut_pos = 800,
    .partial_cut_pos = 150,
    .full_cut_pos = 10,
};
menu_profile_t menu_profile_2_0 = {
    .start_cut_pos = 800,
    .partial_cut_pos = 150,
    .full_cut_pos = 10,
};
menu_profile_t menu_profile_2_5 = {
    .start_cut_pos = 800,
    .partial_cut_pos = 150,
    .full_cut_pos = 10,
};
menu_profile_t menu_profile_3_0 = {
    .start_cut_pos = 800,
    .partial_cut_pos = 150,
    .full_cut_pos = 10,
};
menu_profile_t menu_profile_3_5 = {
    .start_cut_pos = 800,
    .partial_cut_pos = 150,
    .full_cut_pos = 10,
};
menu_profile_t menu_profile_4_0 = {
    .start_cut_pos = 800,
    .partial_cut_pos = 150,
    .full_cut_pos = 10,
};
menu_profile_t menu_profile_4_5 = {
    .start_cut_pos = 800,
    .partial_cut_pos = 150,
    .full_cut_pos = 10,
};
menu_profile_t menu_profile_5_0 = {
    .start_cut_pos = 800,
    .partial_cut_pos = 150,
    .full_cut_pos = 10,
};

static gcryslcd_menu_widget_t start_widget = { .type = GCRYSLCD_MENU_WIDGET_INT, .intWidget = { .base.text = "Start", .min = 1, .max = 10000, .step = 10, .value = NULL }  };
static gcryslcd_menu_widget_t incision_widget = { .type = GCRYSLCD_MENU_WIDGET_INT, .intWidget = { .base.text = "Incision", .min = 1, .max = 10000, .step = 10, .value = NULL }  };
static gcryslcd_menu_widget_t cutting_widget = { .type = GCRYSLCD_MENU_WIDGET_INT, .intWidget = { .base.text = "Cutting", .min = 1, .max = 10000, .step = 10, .value = NULL }, };

static GCRYSLCD_MENU_INIT_PAGE(template_page, &menu_pressets_page,
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_BUTTON, .textWidget = { .text = "Offsets:" }  },
    &start_widget,
    &incision_widget,
    &cutting_widget,
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_SUBMENU, .submenuWidget = { .base.text = "Back", .page = &menu_pressets_page }  },
);

static void set_page_template(menu_profile_t *profile) {
    start_widget.intWidget.value = &profile->start_cut_pos;
    incision_widget.intWidget.value = &profile->partial_cut_pos;
    cutting_widget.intWidget.value = &profile->full_cut_pos;
    
    gcryslcd_menu_set_page(&menu, &template_page);
}

static void action_0_5() {
    set_page_template(&menu_profile_0_5);
}

static void action_1_0() {
    set_page_template(&menu_profile_1_0);
}

static void action_1_5() {
    set_page_template(&menu_profile_1_5);
}

static void action_2_0() {
    set_page_template(&menu_profile_2_0);
}

static void action_2_5() {
    set_page_template(&menu_profile_2_5);
}

static void action_3_0() {
    set_page_template(&menu_profile_3_0);
}

static void action_3_5() {
    set_page_template(&menu_profile_3_5);
}

static void action_4_0() {
    set_page_template(&menu_profile_4_0);
}

static void action_4_5() {
    set_page_template(&menu_profile_4_5);
}

static void action_5_0() {
    set_page_template(&menu_profile_5_0);
}

GCRYSLCD_MENU_INIT_PAGE(menu_pressets_page, &menu_main_page,
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_BUTTON, .buttonWidget = { .base.text = "0.5mm", .nextCursor = &menu_always_true, .action = action_0_5  }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_BUTTON, .buttonWidget = { .base.text = "1.0mm", .nextCursor = &menu_always_true, .action = action_1_0 }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_BUTTON, .buttonWidget = { .base.text = "1.5mm", .nextCursor = &menu_always_true, .action = action_1_5 }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_BUTTON, .buttonWidget = { .base.text = "2.0mm", .nextCursor = &menu_always_true, .action = action_2_0 }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_BUTTON, .buttonWidget = { .base.text = "2.5mm", .nextCursor = &menu_always_true, .action = action_2_5 }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_BUTTON, .buttonWidget = { .base.text = "3.0mm", .nextCursor = &menu_always_true, .action = action_3_0 }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_BUTTON, .buttonWidget = { .base.text = "3.5mm", .nextCursor = &menu_always_true, .action = action_3_5 }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_BUTTON, .buttonWidget = { .base.text = "4.0mm", .nextCursor = &menu_always_true, .action = action_4_0 }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_BUTTON, .buttonWidget = { .base.text = "4.5mm", .nextCursor = &menu_always_true, .action = action_4_5 }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_BUTTON, .buttonWidget = { .base.text = "5.0mm", .nextCursor = &menu_always_true, .action = action_5_0 }  },
    &(gcryslcd_menu_widget_t){ .type = GCRYSLCD_MENU_WIDGET_SUBMENU, .submenuWidget = { .base.text = "Back", .page = &menu_main_page }  },
);