// System
#include <ets_sys.h>
#include <gpio.h>

// Project
#include "buttons.h"

void ICACHE_FLASH_ATTR buttons_init() {
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
	gpio_output_set(0, 0, 0, BIT4);
	gpio_output_set(0, 0, 0, BIT5);
	PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO4_U);
	PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO5_U);
}

bool ICACHE_FLASH_ATTR button_get(button_t btn) {
	if (btn == BUTTON_UP) {
		return !GPIO_INPUT_GET(4);
	} else if (btn == BUTTON_DOWN) {
		return !GPIO_INPUT_GET(5);
	} else if (btn == BUTTON_CENTER) {
		return !GPIO_INPUT_GET(4) && !GPIO_INPUT_GET(5);
	}

	return false;
}
