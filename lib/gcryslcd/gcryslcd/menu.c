#include "menu.h"

static uint32_t get_visible_widget_index(gcryslcd_menu_page_t *page, uint32_t start_idx, uint32_t visible_offset);

static bool get_prev_selectable(gcryslcd_menu_page_t *page, uint32_t current, uint32_t *out_idx);

static bool get_next_selectable(gcryslcd_menu_page_t *page, uint32_t current, uint32_t *out_idx);

static void gcryslcd_menu_handle_navigate(gcryslcd_menu_t *menu, gcryslcd_menu_input_t input);

static void gcryslcd_menu_handle_edit(gcryslcd_menu_t *menu, gcryslcd_menu_input_t input);

static void gcryslcd_get_widget_text_value(const gcryslcd_menu_widget_t *widget, char *buff, uint32_t buff_len, uint8_t display_width);

void gcryslcd_menu_init(gcryslcd_menu_t *menu, gcryslcd_menu_page_t *page) {
    if (!menu || !menu->init.lcd) {
        return;
    }

    gcryslcd_init(menu->init.lcd);
    gcryslcd_menu_set_page(menu, page);
}

void gcryslcd_menu_set_page(gcryslcd_menu_t *menu, gcryslcd_menu_page_t *page) {
    if (!menu || !menu->init.lcd) {
        return;
    }

    menu->_internal.page = page;
    menu->_internal.selected_index = 0;
    menu->_internal.scroll_offset = 0;
    menu->_internal.state = GCRYSLCD_MENU_STATE_NAVIGATE;
    menu->_internal.last_render = 0;

    for (uint32_t i = 0; i < page->widgets_count; i++) {
        if (!page->widgets[i]->baseWidget.hide && !page->widgets[i]->baseWidget.noAvailable) {
            menu->_internal.selected_index = i;

            break;
        }
    }
}

void gcryslcd_menu_handle_input(gcryslcd_menu_t *menu, gcryslcd_menu_input_t input) {
    if (!menu || !menu->init.lcd || !menu->_internal.page || input == GCRYSLCD_MENU_INPUT_NONE) {
        return;
    }

    if (menu->_internal.state == GCRYSLCD_MENU_STATE_NAVIGATE) {
        gcryslcd_menu_handle_navigate(menu, input);
    } else if (menu->_internal.state == GCRYSLCD_MENU_STATE_EDIT) {
        gcryslcd_menu_handle_edit(menu, input);
    }

    gcryslcd_menu_fast_render(menu);
}

void gcryslcd_menu_render(gcryslcd_menu_t *menu) {
    if (!menu || !menu->init.lcd) {
        return;
    }

    uint32_t last = menu->_internal.last_render;
    uint32_t now = gcryslcd_get_millis();

    if (last + 500 <= now) {
        gcryslcd_menu_fast_render(menu);
    }
}

void gcryslcd_menu_fast_render(gcryslcd_menu_t *menu) {
     if (!menu || !menu->init.lcd) {
         return;
     }

    gcryslcd_t *lcd = menu->init.lcd;
    uint8_t lines_to_draw = lcd->init.lines;

    for (uint8_t i = 0; i < lines_to_draw; i++) {
        gcryslcd_menu_update(menu, i);
    }

    menu->_internal.last_render = gcryslcd_get_millis();
}

void gcryslcd_menu_update(gcryslcd_menu_t *menu, uint8_t line) {
    if (!menu || !menu->init.lcd) {
        return;
    }

    gcryslcd_t *lcd = menu->init.lcd;
    uint8_t lcd_cols = lcd->init.cols;
    uint8_t lines_to_draw = lcd->init.lines;

    if (line >= lines_to_draw) {
        return;
    }

    gcryslcd_set_cursor(lcd, line, 0);

    const uint32_t widget_index = get_visible_widget_index(menu->_internal.page, menu->_internal.scroll_offset, line);

    if (widget_index >= menu->_internal.page->widgets_count) {
        for(uint8_t c = 0; c < lcd_cols; c++) {
            gcryslcd_print(lcd, " ");
        }

        return;
    }

    gcryslcd_menu_widget_t *widget = menu->_internal.page->widgets[widget_index];

    const char cursorState = menu->_internal.state == GCRYSLCD_MENU_STATE_NAVIGATE ? '\x7E' : '=';
    const char cursor = widget_index == menu->_internal.selected_index ? cursorState : ' ';

    char buff[GCRYSLCD_PRINTF_BUFFER_LEN] = {0};

    gcryslcd_get_widget_text_value(widget, buff, sizeof(buff), lcd_cols - 2);

    gcryslcd_print_char(lcd, cursor);
    gcryslcd_print(lcd, buff);

    if (line == 0) {
        bool has_prev = false;

        for (int32_t i = (int32_t)menu->_internal.scroll_offset - 1; i >= 0; i--) {
            if (!menu->_internal.page->widgets[i]->baseWidget.hide) {
                has_prev = true;

                break;
            }
        }

        gcryslcd_set_cursor(lcd, line, lcd_cols - 1);
        gcryslcd_print_char(lcd, has_prev ? '^' : ' ');
    }

    if (line == lines_to_draw - 1) {
        bool has_next = (get_visible_widget_index(menu->_internal.page, menu->_internal.scroll_offset, lines_to_draw) < menu->_internal.page->widgets_count);

        gcryslcd_set_cursor(lcd, line, lcd_cols - 1);
        gcryslcd_print_char(lcd, has_next ? 'v' : ' ');
    }
}

static uint32_t get_visible_widget_index(gcryslcd_menu_page_t *page, uint32_t start_idx, uint32_t visible_offset) {
    uint32_t current = start_idx;
    uint32_t visible_count = 0;

    while (current < page->widgets_count) {
        if (!page->widgets[current]->baseWidget.hide) {
            if (visible_count == visible_offset) {
                return current;
            }
            visible_count++;
        }
        current++;
    }
    return page->widgets_count;
}

static bool get_prev_selectable(gcryslcd_menu_page_t *page, uint32_t current, uint32_t *out_idx) {
    if (current == 0) return false;

    for (int32_t i = (int32_t)current - 1; i >= 0; i--) {
        if (!page->widgets[i]->baseWidget.hide && !page->widgets[i]->baseWidget.noAvailable) {
            *out_idx = (uint32_t)i;

            return true;
        }
    }

    return false;
}

static bool get_next_selectable(gcryslcd_menu_page_t *page, uint32_t current, uint32_t *out_idx) {
    for (uint32_t i = current + 1; i < page->widgets_count; i++) {
        if (!page->widgets[i]->baseWidget.hide && !page->widgets[i]->baseWidget.noAvailable) {
            *out_idx = i;

            return true;
        }
    }

    return false;
}

static void gcryslcd_menu_handle_navigate(gcryslcd_menu_t *menu, gcryslcd_menu_input_t input) {
    uint8_t lines_to_draw = menu->init.lcd->init.lines;
    uint32_t *sel = &menu->_internal.selected_index;
    uint32_t *scroll = &menu->_internal.scroll_offset;
    const gcryslcd_menu_page_t *page = menu->_internal.page;

    if (input == GCRYSLCD_MENU_INPUT_UP) {
        uint32_t new_sel;
        if (get_prev_selectable((gcryslcd_menu_page_t*)page, *sel, &new_sel)) {
            *sel = new_sel;

            if (*sel < *scroll) {
                *scroll = *sel;
            }
        }
    } else if (input == GCRYSLCD_MENU_INPUT_DOWN) {
        uint32_t new_sel;

        if (get_next_selectable((gcryslcd_menu_page_t*)page, *sel, &new_sel)) {
            *sel = new_sel;

            uint32_t visible_count = 0;
            for (uint32_t i = *scroll; i <= *sel; i++) {
                if (!page->widgets[i]->baseWidget.hide) visible_count++;
            }

            if (visible_count > lines_to_draw) {
                uint32_t new_scroll = *sel;
                uint32_t vis_seen = 1;

                while (new_scroll > 0 && vis_seen < lines_to_draw) {
                    new_scroll--;
                    if (!page->widgets[new_scroll]->baseWidget.hide) {
                        vis_seen++;
                    }
                }

                *scroll = new_scroll;
            }
        }
    } else if (input == GCRYSLCD_MENU_INPUT_ENTER) {
        gcryslcd_menu_widget_t *widget = menu->_internal.page->widgets[*sel];

        if (widget->baseWidget.hide || widget->baseWidget.noAvailable) {
            return;
        }

        switch (widget->type) {
            case GCRYSLCD_MENU_WIDGET_BUTTON: {
                if (widget->buttonWidget.action) {
                    widget->buttonWidget.action(menu, (gcryslcd_menu_widget_button_t*) &widget->buttonWidget);
                }

                break;
            }
            case GCRYSLCD_MENU_WIDGET_INT:
            case GCRYSLCD_MENU_WIDGET_FLOAT:
            case GCRYSLCD_MENU_WIDGET_BOOL:
            case GCRYSLCD_MENU_WIDGET_SELECT: {
                menu->_internal.state = GCRYSLCD_MENU_STATE_EDIT;

                break;
            }
            case GCRYSLCD_MENU_WIDGET_SUBMENU: {
                gcryslcd_menu_page_t *currentPage = menu->_internal.page;
                gcryslcd_menu_page_t *childPage = widget->submenuWidget.page;

                if (childPage) {
                    childPage->parent = currentPage;

                    gcryslcd_menu_set_page(menu, childPage);

                    if (widget->submenuWidget.onChange) {
                        widget->submenuWidget.onChange(menu, &widget->submenuWidget, currentPage, childPage);
                    }
                }

                break;
            }
            default:
                break;
        }
    } else if (input == GCRYSLCD_MENU_INPUT_BACK) {
        if (page->parent) {
            gcryslcd_menu_set_page(menu, page->parent);
        }
    }
}

static void gcryslcd_menu_handle_edit(gcryslcd_menu_t *menu, gcryslcd_menu_input_t input) {
    if (input == GCRYSLCD_MENU_INPUT_ENTER || input == GCRYSLCD_MENU_INPUT_BACK) {
        menu->_internal.state = GCRYSLCD_MENU_STATE_NAVIGATE;

        return;
    }

    gcryslcd_menu_widget_t *widget = menu->_internal.page->widgets[menu->_internal.selected_index];

    int dir = input == GCRYSLCD_MENU_INPUT_UP ? 1 : (input == GCRYSLCD_MENU_INPUT_DOWN ? -1 : 0);

    if (dir == 0) {
        return;
    }

    switch (widget->type) {
        case GCRYSLCD_MENU_WIDGET_INT: {
            int32_t current = *widget->intWidget.value;
            int32_t step = widget->intWidget.step != 0 ? widget->intWidget.step : 1;
            int32_t next = current + (dir * step);

            if (next > widget->intWidget.max) {
                next = widget->intWidget.max;
            }

            if (next < widget->intWidget.min) {
                next = widget->intWidget.min;
            }

            *widget->intWidget.value = next;

            if (next != current && widget->intWidget.onChange) {
                widget->intWidget.onChange(menu, &widget->intWidget, current, next);
            }

            break;
        }
        case GCRYSLCD_MENU_WIDGET_FLOAT: {
            float current = *widget->floatWidget.value;
            float step = widget->floatWidget.step != 0.0f ? widget->floatWidget.step : 1.0f;
            float next = current + ((float) dir * step);

            if (next > widget->floatWidget.max) {
                next = widget->floatWidget.max;
            }

            if (next < widget->floatWidget.min) {
                next = widget->floatWidget.min;
            }

            *widget->floatWidget.value = next;

            if (next != current && widget->floatWidget.onChange) {
                widget->floatWidget.onChange(menu, &widget->floatWidget, current, next);
            }

            break;
        }
        case GCRYSLCD_MENU_WIDGET_BOOL: {
            bool current = *widget->boolWidget.value;
            bool next = !current;

            *widget->boolWidget.value = next;

            if (widget->boolWidget.onChange) {
                widget->boolWidget.onChange(menu, &widget->boolWidget, current, next);
            }

            break;
        }
        case GCRYSLCD_MENU_WIDGET_SELECT: {
            uint32_t current = *widget->selectWidget.selected_index;
            uint32_t count = widget->selectWidget.values_count;
            uint32_t next = current;

            if (dir > 0) {
                next = current + 1 < count ? current + 1 : 0;
            } else {
                next = current > 0 ? current - 1 : count - 1;
            }

            *widget->selectWidget.selected_index = next;

            if (next != current && widget->selectWidget.onChange) {
                widget->selectWidget.onChange(menu, (gcryslcd_menu_widget_select_t*)&widget->selectWidget, current, next);
            }

            break;
        }
        default:
            break;
    }
}

static void gcryslcd_get_widget_text_value(const gcryslcd_menu_widget_t *widget, char *buff, uint32_t buff_len, uint8_t display_width) {
    if (!buff || buff_len == 0 || display_width == 0) {
        return;
    }

    memset(buff, 0, buff_len);

    char val_str[GCRYSLCD_PRINTF_BUFFER_LEN] = {0};

    switch (widget->type) {
        case GCRYSLCD_MENU_WIDGET_BUTTON: {
            bool nextCursor = widget->buttonWidget.nextCursor ? *widget->buttonWidget.nextCursor : false;

            if (nextCursor) {
                snprintf(val_str, sizeof(val_str), ">");
            }

            break;
        }
        case GCRYSLCD_MENU_WIDGET_INT: {
            snprintf(val_str, sizeof(val_str), "[%ld]", *widget->intWidget.value);

            break;
        }
        case GCRYSLCD_MENU_WIDGET_FLOAT: {
            const float val = *widget->floatWidget.value;
            int32_t int_part = (int32_t) val;
            int32_t frac_part = (int32_t) ((val - (float) int_part) * 100);

            if (frac_part < 0) {
                frac_part = -frac_part;
            }

            if (val < 0 && int_part == 0) {
                snprintf(val_str, sizeof(val_str), "[-0.%02ld]", frac_part);
            } else {
                snprintf(val_str, sizeof(val_str), "[%ld.%02ld]", int_part, frac_part);
            }

            break;
        }
        case GCRYSLCD_MENU_WIDGET_BOOL: {
            snprintf(val_str, sizeof(val_str), "[%s]", *widget->boolWidget.value ? "X" : " ");

            break;
        }
        case GCRYSLCD_MENU_WIDGET_SELECT: {
            gcryslcd_menu_widget_select_t select = widget->selectWidget;

            snprintf(val_str, sizeof(val_str), "<%s>", select.values[*select.selected_index % select.values_count]);

            break;
        }
        case GCRYSLCD_MENU_WIDGET_SUBMENU: {
            bool nextCursor = widget->submenuWidget.nextCursor ? *widget->submenuWidget.nextCursor : true;

            if (nextCursor) {
                snprintf(val_str, sizeof(val_str), ">");
            }

            break;
        }
        default:
            break;
    }

    uint32_t val_len = strlen(val_str);

    const char *base_text = widget->baseWidget.text ? widget->baseWidget.text : "";
    uint32_t text_len = strlen(base_text);

    uint32_t val_space = val_len > display_width ? display_width : val_len;

    uint32_t available_for_text = display_width - val_space;
    uint32_t text_space = text_len;

    uint8_t min_padding = val_space > 0 && available_for_text > 0 ? 1 : 0;

    if (text_space > available_for_text - min_padding) {
        text_space = available_for_text > min_padding ? available_for_text - min_padding : 0;
    }

    uint32_t padding = display_width - text_space - val_space;

    uint32_t out_idx = 0;

    for (uint32_t i = 0; i < text_space && out_idx < buff_len - 1; i++) {
        buff[out_idx++] = base_text[i];
    }

    for (uint32_t i = 0; i < padding && out_idx < buff_len - 1; i++) {
        buff[out_idx++] = ' ';
    }

    for (uint32_t i = 0; i < val_space && out_idx < buff_len - 1; i++) {
        buff[out_idx++] = val_str[i];
    }

    buff[out_idx] = '\0';
}
