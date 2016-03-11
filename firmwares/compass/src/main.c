// System
#include <user_interface.h>
#include <math.h>
#include <os_type.h>
#include <ets_sys.h>
#include <osapi.h>
#include <gpio.h>

// Configuration
#include "user_config.h"

// Project
#include "httpclient.h"
#include "i2c_master.h"
#include "buttons.h"
#include "mpu9250.h"
#include "ssd1306.h"
#include "rv3029.h"
#include "vector.h"
#include "util.h"

os_timer_t magnet_timer;
vec3f calib_bias;

#define COMPASS_CENTER_X 32
#define COMPASS_CENTER_Y 32
#define COMPASS_RADIUS 30
void ICACHE_FLASH_ATTR compass_draw(float angle) {
	float a;
	ssd1306_setpixel(COMPASS_CENTER_X, COMPASS_CENTER_Y, true);
	ssd1306_setpixel(COMPASS_CENTER_X + 1, COMPASS_CENTER_Y, true);
	ssd1306_setpixel(COMPASS_CENTER_X, COMPASS_CENTER_Y + 1, true);
	ssd1306_setpixel(COMPASS_CENTER_X - 1, COMPASS_CENTER_Y, true);
	ssd1306_setpixel(COMPASS_CENTER_X, COMPASS_CENTER_Y - 1, true);

	for (a = angle; a < angle + 2 * PI; a += PI / 8) {
		float startx =  ((a == angle) ? COMPASS_RADIUS - 15 : COMPASS_RADIUS - 5) * sin(a) + COMPASS_CENTER_X;
		float starty = -((a == angle) ? COMPASS_RADIUS - 15 : COMPASS_RADIUS - 5) * cos(a) + COMPASS_CENTER_Y;
		float endx =  COMPASS_RADIUS * sin(a) + COMPASS_CENTER_X;
		float endy = -COMPASS_RADIUS * cos(a) + COMPASS_CENTER_Y;
		if (a == angle)
			ssd1306_text_small(startx - 3 - (endx - startx) / 2, starty - 4 - (endy - starty) / 2, "N");
		ssd1306_line(startx, starty, endx, endy, true);
	}
}

void ICACHE_FLASH_ATTR magnet_timer_cb() {
	vec3s16 field;
	ak8963_get_field(&field);
	field.x -= calib_bias.x;
	field.y -= calib_bias.y;
	field.z -= calib_bias.z;
	//os_printf("Measurement: { x = %d, y = %d, z = %d }\r\n", (int16_t)(field.x), (int16_t)(field.y), (int16_t)(field.z));
	angles2f north = vec3s16_to_angles2f(field);
	//os_printf("North: { yaw = %d, pitch = %d }\r\n", (int16_t)(north.yaw * 180 / PI), (int16_t)(north.pitch * 180 / PI));
	ak8963_set_opmode(AK8963_SINGLE, AK8963_16BIT);

	ssd1306_clear();
	compass_draw(north.yaw);
	ssd1306_line(70, 0, 70, 63, true);
	ssd1306_text_small(75, 24, "Inclin.:");
	char incdeg[6];
	os_sprintf(incdeg, "%d°", (int16_t)(north.pitch * 180 / PI));
	ssd1306_text_small(75, 32, incdeg);
	ssd1306_flip();
}

void ICACHE_FLASH_ATTR on_ak8963_calibration(vec3f bias) {
	os_printf("Bias: { x = %d, y = %d, z = %d }\r\n", (int16_t)bias.x, (int16_t)bias.y, (int16_t)bias.z);
	calib_bias.x = bias.x;
	calib_bias.y = bias.y;
	calib_bias.z = bias.z;

	/*** WiFi sleep ***/
	wifi_station_disconnect();
	wifi_set_opmode(NULL_MODE);
	wifi_fpm_set_sleep_type(MODEM_SLEEP_T);
	wifi_fpm_open();
	wifi_fpm_do_sleep(0xfffffff);

	os_timer_setfn(&magnet_timer, (os_timer_func_t *)magnet_timer_cb, NULL);
	os_timer_arm(&magnet_timer, 300, true);
}

void ICACHE_FLASH_ATTR user_init(void) {
	uart_div_modify(0, UART_CLK_FREQ / BAUD);

	wifi_set_opmode(STATION_MODE);
	wifi_station_connect();

	/*** I²C init ***/
	i2c_master_gpio_init();
	mpu9250_init();
	ak8963_init();

	os_delay_us(10000);
	i2c_scanbus();

	/*** LCD Init ***/
	ssd1306_init(0);
	ssd1306_text_small(10, 10, "Init...");
	ssd1306_flip();

	// ~Charge input
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);
	gpio_output_set(0, 0, 0, BIT13);

	// Button inputs
	buttons_init();

	ak8963_calibration_start(10000, on_ak8963_calibration);

	os_printf("Startup\r\n");
}
