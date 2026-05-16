#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "lcd.h"

#ifndef GCRYSLCD_MENU_RUN_LINE_DELAY
    #define GCRYSLCD_MENU_RUN_LINE_DELAY 500
#endif

typedef struct gcryslcd_menu_page_s gcryslcd_menu_page_t;
typedef struct gcryslcd_menu_s gcryslcd_menu_t;

typedef struct {
    char *text;
    bool *noAvailable;
    bool *hide;
} gcryslcd_menu_widget_base_t;

typedef gcryslcd_menu_widget_base_t gcryslcd_menu_widget_text_t;

typedef struct gcryslcd_menu_widget_button_s gcryslcd_menu_widget_button_t;
struct gcryslcd_menu_widget_button_s {
    gcryslcd_menu_widget_base_t base;
    bool *nextCursor;
    void (*action)(gcryslcd_menu_t *menu, gcryslcd_menu_widget_button_t *widget);
};

typedef struct gcryslcd_menu_widget_int_s gcryslcd_menu_widget_int_t;
struct gcryslcd_menu_widget_int_s {
    gcryslcd_menu_widget_base_t base;
    int32_t *value;
    int32_t min;
    int32_t max;
    int32_t step;
    void (*onChange)(gcryslcd_menu_t *menu, gcryslcd_menu_widget_int_t *widget, int32_t oldValue, int32_t newValue);
};

typedef struct gcryslcd_menu_widget_float_s gcryslcd_menu_widget_float_t;
struct gcryslcd_menu_widget_float_s {
    gcryslcd_menu_widget_base_t base;
    float *value;
    float min;
    float max;
    float step;
    void (*onChange)(gcryslcd_menu_t *menu, gcryslcd_menu_widget_float_t *widget, float oldValue, float newValue);
};

typedef struct gcryslcd_menu_widget_bool_s gcryslcd_menu_widget_bool_t;
struct gcryslcd_menu_widget_bool_s {
    gcryslcd_menu_widget_base_t base;
    bool *value;
    void (*onChange)(gcryslcd_menu_t *menu, gcryslcd_menu_widget_bool_t *widget, bool oldValue, bool newValue);
};

typedef struct gcryslcd_menu_widget_select_s gcryslcd_menu_widget_select_t;
struct gcryslcd_menu_widget_select_s {
    gcryslcd_menu_widget_base_t base;
    const char **values;
    uint32_t values_count;
    uint32_t *selected_index;
    void (*onChange)(gcryslcd_menu_t *menu, gcryslcd_menu_widget_select_t *widget, bool oldValue, bool newValue);
};

typedef struct gcryslcd_menu_widget_submenu_s gcryslcd_menu_widget_submenu_t;
struct gcryslcd_menu_widget_submenu_s {
    gcryslcd_menu_widget_base_t base;
    gcryslcd_menu_page_t *page;
    bool *nextCursor;
    void (*onChange)(gcryslcd_menu_t *menu, gcryslcd_menu_widget_submenu_t *widget, gcryslcd_menu_page_t *oldValue, gcryslcd_menu_page_t *newValue);
};

typedef enum {
    GCRYSLCD_MENU_WIDGET_TEXT,
    GCRYSLCD_MENU_WIDGET_BUTTON,
    GCRYSLCD_MENU_WIDGET_INT,
    GCRYSLCD_MENU_WIDGET_FLOAT,
    GCRYSLCD_MENU_WIDGET_BOOL,
    GCRYSLCD_MENU_WIDGET_SELECT,
    GCRYSLCD_MENU_WIDGET_SUBMENU
} gcryslcd_menu_widget_type_t;

typedef struct {
    gcryslcd_menu_widget_type_t type;
    union {
        gcryslcd_menu_widget_base_t baseWidget;
        gcryslcd_menu_widget_text_t textWidget;
        gcryslcd_menu_widget_button_t buttonWidget;
        gcryslcd_menu_widget_int_t intWidget;
        gcryslcd_menu_widget_float_t floatWidget;
        gcryslcd_menu_widget_bool_t boolWidget;
        gcryslcd_menu_widget_select_t selectWidget;
        gcryslcd_menu_widget_submenu_t submenuWidget;
    };
} gcryslcd_menu_widget_t;

struct gcryslcd_menu_page_s {
    gcryslcd_menu_widget_t **widgets;
    uint8_t widgets_count;
    gcryslcd_menu_page_t *parent;
};

typedef enum {
    GCRYSLCD_MENU_STATE_NAVIGATE,
    GCRYSLCD_MENU_STATE_EDIT
} gcryslcd_menu_state_t;

typedef enum {
    GCRYSLCD_MENU_INPUT_NONE,
    GCRYSLCD_MENU_INPUT_UP,
    GCRYSLCD_MENU_INPUT_DOWN,
    GCRYSLCD_MENU_INPUT_ENTER,
    GCRYSLCD_MENU_INPUT_BACK
} gcryslcd_menu_input_t;

typedef struct {
    gcryslcd_t *lcd;
} gcryslcd_menu_init_t;

typedef struct {
    gcryslcd_menu_page_t* page;
    uint32_t selected_index;
    uint32_t scroll_offset;
    gcryslcd_menu_state_t state;
    uint32_t last_render;

} gcryslcd_menu_internal_t;

struct gcryslcd_menu_s {
    gcryslcd_menu_init_t init;

    gcryslcd_menu_internal_t _internal;
};

#define GCRYSLCD_MENU_DEFINE_PAGE(page_name) gcryslcd_menu_page_t page_name
#define GCRYSLCD_MENU_INIT_PAGE(page_name, parent_page, ...) \
gcryslcd_menu_page_t page_name = { \
    .parent = parent_page, \
    .widgets = (gcryslcd_menu_widget_t*[]) { __VA_ARGS__ }, \
    .widgets_count = sizeof((gcryslcd_menu_widget_t*[]) { __VA_ARGS__ }) / sizeof(gcryslcd_menu_widget_t*) \
}

void gcryslcd_menu_init(gcryslcd_menu_t *lcd, gcryslcd_menu_page_t *page);

void gcryslcd_menu_set_page(gcryslcd_menu_t *menu, gcryslcd_menu_page_t *page);

void gcryslcd_menu_handle_input(gcryslcd_menu_t *menu, gcryslcd_menu_input_t input);

void gcryslcd_menu_render(gcryslcd_menu_t *menu);

void gcryslcd_menu_fast_render(gcryslcd_menu_t *menu);

void gcryslcd_menu_update(gcryslcd_menu_t *menu, uint8_t line);