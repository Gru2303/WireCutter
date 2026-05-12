#include "gbutton.h"

void gbutton_clear(gbutton_t *handle) {
	if (handle->_internal.flags.bits.clks_r) {
		handle->_internal.clicks = 0;
	}

	if (handle->_internal.flags.bits.clks_r ||
		handle->_internal.flags.bits.stp_r ||
		handle->_internal.flags.bits.prs_r ||
		handle->_internal.flags.bits.hld_r ||
		handle->_internal.flags.bits.rel_r) {
		handle->_internal.flags.bits.clks_r = 0;
		handle->_internal.flags.bits.stp_r = 0;
		handle->_internal.flags.bits.prs_r = 0;
		handle->_internal.flags.bits.hld_r = 0;
		handle->_internal.flags.bits.rel_r = 0;
	}
}

bool gbutton_raw_tick(gbutton_t *handle) {
	if (!handle->init.gpio.read) {
		return false;
	}

	bool click = handle->init.gpio.read();

	if (handle->_internal.flags.bits.bisr) {
		handle->_internal.flags.bits.bisr = 0;
		click = true;
	} else {
		click ^= handle->_internal.flags.bits.inv;
	}

	if (!handle->_internal.flags.bits.busy) {
		if (click) {
			handle->_internal.flags.bits.busy = 1;
		} else {
			return false;
		}
	}

	uint16_t ms = gbutton_get_millis();
	uint16_t deb = ms - handle->_internal.timer;

	if (click) {
		if (!handle->_internal.flags.bits.prs) {
			if (!handle->_internal.flags.bits.deb && handle->init.time.deb) {
				handle->_internal.flags.bits.deb = 1;
				handle->_internal.timer = ms;
			} else if (deb >= handle->init.time.deb || !handle->init.time.deb) {
				handle->_internal.flags.bits.prs = 1;
				handle->_internal.flags.bits.prs_r = 1;
				handle->_internal.timer = ms;
			}
		} else {
			if (!handle->_internal.flags.bits.ehld) {
				if (!handle->_internal.flags.bits.hld) {
					if (deb >= handle->init.time.hold) {
						handle->_internal.flags.bits.hld = 1;
						handle->_internal.flags.bits.hld_r = 1;
						handle->_internal.timer = ms;
					}
				} else {
					if (deb >= (handle->_internal.flags.bits.stp ? handle->init.time.step : handle->init.time.hold)) {
						handle->_internal.flags.bits.stp = 1;
						handle->_internal.flags.bits.stp_r = 1;
						handle->_internal.timer = ms;
					}
				}
			}
		}
	} else {
		if (handle->_internal.flags.bits.prs) {
			if (deb >= handle->init.time.deb) {
				if (!handle->_internal.flags.bits.hld) {
					handle->_internal.clicks++;
				}

				if (handle->_internal.flags.bits.ehld) {
					handle->_internal.clicks = 0;
				}

				handle->_internal.flags.bits.rel = 1;
				handle->_internal.flags.bits.rel_r = 1;
				handle->_internal.flags.bits.prs = 0;
			}
		} else if (handle->_internal.flags.bits.rel) {
			if (!handle->_internal.flags.bits.ehld) {
				handle->_internal.flags.bits.rel_r = 1;
			}

			handle->_internal.flags.bits.rel = 0;
			handle->_internal.flags.bits.ehld = 0;

			handle->_internal.timer = ms;
		} else if (handle->_internal.clicks) {
			if (handle->_internal.flags.bits.hld || handle->_internal.flags.bits.stp || deb >= handle->init.time.click) {
				handle->_internal.flags.bits.clks_r = 1;
			}
		} else {
			handle->_internal.flags.bits.hld = 0;
			handle->_internal.flags.bits.stp = 0;
			handle->_internal.flags.bits.busy = 0;
			handle->_internal.flags.bits.tout = 1;

			handle->_internal.timer = ms;
		}

		if (handle->_internal.flags.bits.deb) {
			handle->_internal.flags.bits.deb = 0;
		}
	}

	return handle->_internal.flags.bits.clks_r || handle->_internal.flags.bits.prs_r || handle->_internal.flags.bits.hld_r || handle->_internal.flags.bits.stp_r || handle->_internal.flags.bits.rel_r;
}

bool gbutton_tick(gbutton_t *handle) {
	gbutton_clear(handle);

	bool status = gbutton_raw_tick(handle);

	if (status) {
		if (handle->init.callback.event) {
			handle->init.callback.event();
		}
	}

	return status;
}