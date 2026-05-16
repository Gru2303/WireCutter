#pragma once

#include <stdint.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdarg.h>

// commands
#define GCRYSLCD_CLEAR_DISPLAY 0x01
#define GCRYSLCD_RETURN_HOME 0x02
#define GCRYSLCD_ENTRY_MODE_SET 0x04
#define GCRYSLCD_DISPLAY_CONTROL 0x08
#define GCRYSLCD_CURSOR_SHIFT 0x10
#define GCRYSLCD_FUNCTION_SET 0x20
#define GCRYSLCD_SET_CGRAM_ADDR 0x40
#define GCRYSLCD_SET_DDRAM_ADDR 0x80

// flags for display entry mode
#define GCRYSLCD_ENTRY_RIGHT 0x00
#define GCRYSLCD_ENTRY_LEFT 0x02
#define GCRYSLCD_ENTRY_SHIFT_INCREMENT 0x01
#define GCRYSLCD_ENTRY_SHIFT_DECREMENT 0x00

// flags for display on/off control
#define GCRYSLCD_DISPLAY_ON 0x04
#define GCRYSLCD_DISPLAY_OFF 0x00
#define GCRYSLCD_CURSOR_ON 0x02
#define GCRYSLCD_CURSOR_OFF 0x00
#define GCRYSLCD_BLINK_ON 0x01
#define GCRYSLCD_BLINK_OFF 0x00

// flags for display/cursor shift
#define GCRYSLCD_DISPLAY_MOVE 0x08
#define GCRYSLCD_CURSOR_MOVE 0x00
#define GCRYSLCD_MOVE_RIGHT 0x04
#define GCRYSLCD_MOVE_LEFT 0x00

// flags for function set
#define GCRYSLCD_4BITMODE 0x00
#define GCRYSLCD_8BITMODE 0x10
#define GCRYSLCD_1LINE 0x00
#define GCRYSLCD_2LINE 0x08
#define GCRYSLCD_5x8DOTS 0x00
#define GCRYSLCD_5x10DOTS 0x04

#define GCRYSLCD_PRINTF_BUFFER_LEN 24

typedef struct {
    void (*send_cmd)(uint8_t data);
    void (*send_data)(uint8_t data);
} gcryslcd_hw_t;

typedef enum {
    GCRYSLCD_8BIT_MODE = 0x00,
    GCRYSLCD_4BIT_MODE = 0x01,
} gcryslcd_mode_t;

typedef struct {
    gcryslcd_hw_t hw;

    gcryslcd_mode_t mode;
    uint8_t cols;
    uint8_t lines;
    uint8_t dotSize;
} gcryslcd_init_t;

typedef struct {
    uint8_t displayFunction;
    uint8_t displayControl;
    uint8_t displayMode;
} gcryslcd_internal_t;

typedef struct {
    gcryslcd_init_t init;

    gcryslcd_internal_t _internal;
} gcryslcd_t;

extern uint32_t gcryslcd_get_millis();

void gcryslcd_init(gcryslcd_t *lcd);

void gcryslcd_clear(gcryslcd_t *lcd);

void gcryslcd_home(gcryslcd_t *lcd);

void gcryslcd_enable_display(gcryslcd_t *lcd);

void gcryslcd_disable_display(gcryslcd_t *lcd);

void gcryslcd_enable_cursor(gcryslcd_t *lcd);

void gcryslcd_disable_cursor(gcryslcd_t *lcd);

void gcryslcd_enable_cursor_blink(gcryslcd_t *lcd);

void gcryslcd_disable_cursor_blink(gcryslcd_t *lcd);

void gcryslcd_set_cursor(gcryslcd_t *lcd, int row, int col);

void gcryslcd_print_char(gcryslcd_t *lcd, const char c);

void gcryslcd_print(gcryslcd_t *lcd, const char *text);

void gcryslcd_printf(gcryslcd_t *lcd, const char *fmt, ...);