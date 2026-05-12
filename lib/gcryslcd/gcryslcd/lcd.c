#include "lcd.h"

static void gcryslcd_delay(uint32_t ms) {
    uint32_t start = gcryslcd_get_millis();
    uint32_t wait = ms;

    if (wait < 0xFFFFFFFFU) {
        wait += 1;
    }

    while((gcryslcd_get_millis() - start) < wait){
    }
}

void gcryslcd_init(gcryslcd_t *lcd) {
    if (!lcd) {
        return;
    }

    if (lcd->init.mode == GCRYSLCD_4BIT_MODE) {
        lcd->_internal.displayFunction = GCRYSLCD_4BITMODE | GCRYSLCD_1LINE | GCRYSLCD_5x8DOTS;
    } else {
        lcd->_internal.displayFunction = GCRYSLCD_8BITMODE | GCRYSLCD_1LINE | GCRYSLCD_5x8DOTS;
    }

    if (lcd->init.lines > 1) {
        lcd->_internal.displayFunction |= GCRYSLCD_2LINE;
    }

    if (lcd->init.mode == GCRYSLCD_4BIT_MODE) {
        gcryslcd_delay(50);

        lcd->init.hw.send_cmd(0x03 << 4);
        gcryslcd_delay(5);

        lcd->init.hw.send_cmd(0x03 << 4);
        gcryslcd_delay(1);

        lcd->init.hw.send_cmd(0x03 << 4);
        gcryslcd_delay(10);

        lcd->init.hw.send_cmd(0x02 << 4);
        gcryslcd_delay(10);
    } else {
        gcryslcd_delay(50);

        lcd->init.hw.send_cmd(GCRYSLCD_FUNCTION_SET | lcd->_internal.displayFunction);
        gcryslcd_delay(5);

        lcd->init.hw.send_cmd(GCRYSLCD_FUNCTION_SET | lcd->_internal.displayFunction);
        gcryslcd_delay(2);

        lcd->init.hw.send_cmd(GCRYSLCD_FUNCTION_SET | lcd->_internal.displayFunction);
    }

    lcd->init.hw.send_cmd(GCRYSLCD_FUNCTION_SET | lcd->_internal.displayFunction);
    gcryslcd_delay(10);

    lcd->_internal.displayControl = GCRYSLCD_DISPLAY_ON | GCRYSLCD_CURSOR_OFF | GCRYSLCD_BLINK_OFF;

    gcryslcd_enable_display(lcd);
    gcryslcd_clear(lcd);

    lcd->_internal.displayMode = GCRYSLCD_ENTRY_LEFT | GCRYSLCD_ENTRY_SHIFT_DECREMENT;

    lcd->init.hw.send_cmd(GCRYSLCD_ENTRY_MODE_SET| lcd->_internal.displayMode);
    gcryslcd_delay(10);
}

void gcryslcd_clear(gcryslcd_t *lcd) {
    if (!lcd) {
        return;
    }

    lcd->init.hw.send_cmd(GCRYSLCD_CLEAR_DISPLAY);
    gcryslcd_delay(2);
}

void gcryslcd_home(gcryslcd_t *lcd) {
    if (!lcd) {
        return;
    }

    lcd->init.hw.send_cmd(GCRYSLCD_RETURN_HOME);
}

void gcryslcd_enable_display(gcryslcd_t *lcd) {
    if (!lcd) {
        return;
    }

    lcd->_internal.displayControl |= GCRYSLCD_DISPLAY_ON;
    lcd->init.hw.send_cmd(GCRYSLCD_DISPLAY_CONTROL | lcd->_internal.displayControl);
    gcryslcd_delay(2);
}

void gcryslcd_disable_display(gcryslcd_t *lcd) {
    if (!lcd) {
        return;
    }

    lcd->_internal.displayControl &= ~GCRYSLCD_DISPLAY_ON;
    lcd->init.hw.send_cmd(GCRYSLCD_DISPLAY_CONTROL | lcd->_internal.displayControl);
    gcryslcd_delay(2);
}

void gcryslcd_enable_cursor(gcryslcd_t *lcd) {
    if (!lcd) {
        return;
    }

    lcd->_internal.displayControl |= GCRYSLCD_CURSOR_ON;
    lcd->init.hw.send_cmd(GCRYSLCD_DISPLAY_CONTROL | lcd->_internal.displayControl);
    gcryslcd_delay(2);
}

void gcryslcd_disable_cursor(gcryslcd_t *lcd) {
    if (!lcd) {
        return;
    }

    lcd->_internal.displayControl &= ~GCRYSLCD_CURSOR_ON;
    lcd->init.hw.send_cmd(GCRYSLCD_DISPLAY_CONTROL | lcd->_internal.displayControl);
    gcryslcd_delay(2);
}

void gcryslcd_enable_cursor_blink(gcryslcd_t *lcd) {
    if (!lcd) {
        return;
    }

    lcd->_internal.displayControl |= GCRYSLCD_BLINK_ON;
    lcd->init.hw.send_cmd(GCRYSLCD_DISPLAY_CONTROL | lcd->_internal.displayControl);
    gcryslcd_delay(2);
}

void gcryslcd_disable_cursor_blink(gcryslcd_t *lcd) {
    if (!lcd) {
        return;
    }

    lcd->_internal.displayControl &= ~GCRYSLCD_BLINK_ON;
    lcd->init.hw.send_cmd(GCRYSLCD_DISPLAY_CONTROL | lcd->_internal.displayControl);
    gcryslcd_delay(2);
}

void gcryslcd_set_cursor(gcryslcd_t *lcd, int row, int col) {
    if (!lcd) {
        return;
    }

    const int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };

    if (row > lcd->init.lines) {
        row = lcd->init.lines - 1;
    }

    lcd->init.hw.send_cmd(GCRYSLCD_SET_DDRAM_ADDR | (col + row_offsets[row]));
}

void gcryslcd_print_char(gcryslcd_t *lcd, const char c) {
    if (!lcd) {
        return;
    }

    lcd->init.hw.send_data(c);
}

void gcryslcd_print(gcryslcd_t *lcd, const char *text) {
    if (!lcd) {
        return;
    }

    if (!text) {
        return;
    }

    while (*text) {
        lcd->init.hw.send_data(*text++);
    }
}

void gcryslcd_printf(gcryslcd_t *lcd, const char *fmt, ...) {
    if (!lcd) {
        return;
    }

    if (!fmt) {
        return;
    }

    if (strlen(fmt) >= GCRYSLCD_PRINTF_BUFFER_LEN) {
        return;
    }

    char buffer[GCRYSLCD_PRINTF_BUFFER_LEN];

    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    gcryslcd_print(lcd, buffer);
}

