#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef union {
	uint16_t whole;

	struct {
		uint8_t clks_r : 1;

		uint8_t prs_r : 1;
		uint8_t hld_r : 1;
		uint8_t stp_r : 1;
		uint8_t rel_r : 1;

		uint8_t prs : 1;
		uint8_t hld : 1;
		uint8_t stp : 1;
		uint8_t rel : 1;

		uint8_t busy : 1;
		uint8_t deb : 1;
		uint8_t tout : 1;
		uint8_t inv : 1;
		uint8_t bisr : 1;
		uint8_t ehld : 1;
	} bits;
} gbutton_flags_t;

typedef struct {
	bool (*read)(void);
} gbutton_gpio_t;

typedef struct {
	void (*event)(void);
} gbutton_callback_t;

typedef struct {
	uint16_t hold;
	uint16_t step;
	uint16_t click;
	uint8_t deb;
} gbutton_time_t;

typedef struct {
	gbutton_gpio_t gpio;

	gbutton_time_t time;

	gbutton_callback_t callback;
} gbutton_init_t;

typedef struct {
	uint16_t clicks;

	uint32_t timer;

	gbutton_flags_t flags;
} gbutton_internal_t;

typedef struct {
	gbutton_init_t init;

	gbutton_internal_t _internal;
} gbutton_t;

extern uint32_t gbutton_get_millis();

__attribute__((always_inline)) inline void gbutton_set_hold_time(gbutton_t *handle, uint16_t time) {
	handle->init.time.hold = time;
}

__attribute__((always_inline)) inline void gbutton_set_step_time(gbutton_t *handle, uint16_t time) {
	handle->init.time.step = time;
}

__attribute__((always_inline)) inline void gbutton_set_click_time(gbutton_t *handle, uint16_t time) {
	handle->init.time.click = time;
}

__attribute__((always_inline)) inline void gbutton_set_deb_time(gbutton_t *handle, uint16_t time) {
	handle->init.time.deb = time;
}

__attribute__((always_inline)) inline void gbutton_set_btn_level(gbutton_t *handle, uint8_t level) {
	handle->_internal.flags.bits.inv = level;
}

__attribute__((always_inline)) inline void gbutton_reset(gbutton_t *handle) {
	handle->_internal.flags.bits.inv = 0;
	handle->_internal.clicks = 0;
}

__attribute__((always_inline)) inline void gbutton_skip_events(gbutton_t *handle) {
	handle->_internal.flags.bits.ehld = 1;
}

__attribute__((always_inline)) inline bool gbutton_has_clicks(gbutton_t *handle) {
	return handle->_internal.flags.bits.clks_r && !handle->_internal.flags.bits.hld;
}

__attribute__((always_inline)) inline uint8_t gbutton_get_clicks(gbutton_t *handle) {
	return handle->_internal.clicks;
}

__attribute__((always_inline)) inline bool gbutton_press(gbutton_t *handle) {
	return handle->_internal.flags.bits.prs_r;
}

__attribute__((always_inline)) inline bool gbutton_press_with_clicks(gbutton_t *handle, uint16_t clicks) {
	return (handle->_internal.clicks == clicks && gbutton_press(handle));
}

__attribute__((always_inline)) inline bool gbutton_release(gbutton_t *handle) {
	return handle->_internal.flags.bits.rel_r && handle->_internal.flags.bits.rel;
}

__attribute__((always_inline)) inline bool gbutton_release_with_clicks(gbutton_t *handle, uint16_t clicks) {
	return (handle->_internal.clicks == clicks && gbutton_release(handle));
}

__attribute__((always_inline)) inline bool gbutton_click(gbutton_t *handle) {
	return handle->_internal.flags.bits.rel_r && !handle->_internal.flags.bits.rel && !handle->_internal.flags.bits.hld;
}

__attribute__((always_inline)) inline bool gbutton_click_with_clicks(gbutton_t *handle, uint16_t clicks) {
	return (handle->_internal.clicks == clicks && gbutton_click(handle));
}

__attribute__((always_inline)) inline bool gbutton_hold(gbutton_t *handle) {
	return handle->_internal.flags.bits.hld_r;
}

__attribute__((always_inline)) inline bool gbutton_hold_with_clicks(gbutton_t *handle, uint16_t clicks) {
	return (handle->_internal.clicks == clicks && gbutton_hold(handle));
}

__attribute__((always_inline)) inline bool gbutton_pressing(gbutton_t *handle) {
	return handle->_internal.flags.bits.prs;
}

__attribute__((always_inline)) inline bool gbutton_pressing_with_clicks(gbutton_t *handle, uint16_t clicks) {
	return (handle->_internal.clicks == clicks && gbutton_pressing(handle));
}

__attribute__((always_inline)) inline bool gbutton_holding(gbutton_t *handle) {
	return handle->_internal.flags.bits.prs && handle->_internal.flags.bits.hld;
}

__attribute__((always_inline)) inline bool gbutton_holding_with_clicks(gbutton_t *handle, uint16_t clicks) {
	return (handle->_internal.clicks == clicks && gbutton_holding(handle));
}

__attribute__((always_inline)) inline bool gbutton_step(gbutton_t *handle) {
	return handle->_internal.flags.bits.stp_r;
}

__attribute__((always_inline)) inline bool gbutton_step_with_clicks(gbutton_t *handle, uint16_t clicks) {
	return (handle->_internal.clicks == clicks && gbutton_step(handle));
}

__attribute__((always_inline)) inline bool gbutton_release_hold(gbutton_t *handle) {
	return handle->_internal.flags.bits.rel_r && handle->_internal.flags.bits.hld && !handle->_internal.flags.bits.rel && !handle->_internal.flags.bits.hld;
}

__attribute__((always_inline)) inline bool gbutton_release_hold_with_clicks(gbutton_t *handle, uint16_t clicks) {
	return (handle->_internal.clicks == clicks && (handle->_internal.flags.bits.clks_r && handle->_internal.flags.bits.hld && !handle->_internal.flags.bits.stp));
}

__attribute__((always_inline)) inline bool gbutton_release_step(gbutton_t *handle) {
	return handle->_internal.flags.bits.rel_r && handle->_internal.flags.bits.stp && !handle->_internal.flags.bits.rel;
}

__attribute__((always_inline)) inline bool gbutton_release_step_with_clicks(gbutton_t *handle, uint16_t clicks) {
	return (handle->_internal.clicks == clicks && (handle->_internal.flags.bits.clks_r && handle->_internal.flags.bits.stp));
}

__attribute__((always_inline)) inline bool gbutton_busy(gbutton_t *handle) {
	return handle->_internal.flags.bits.busy;
}

__attribute__((always_inline)) inline bool gbutton_timeout(gbutton_t *handle, uint32_t timeout) {
	if (handle->_internal.flags.bits.tout && (gbutton_get_millis() - handle->_internal.timer > timeout)) {
		handle->_internal.flags.bits.tout = 0;

		return true;
	}

	return false;
}

void gbutton_clear(gbutton_t *handle);

bool gbutton_raw_tick(gbutton_t *handle);

bool gbutton_tick(gbutton_t *handle);