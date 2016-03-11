// System
#include <ets_sys.h>
#include <osapi.h>
#include <gpio.h>
#include <os_type.h>
#include <user_interface.h>

// Configuration
#include "user_config.h"

// Project
#include "i2c_master.h"
#include "ssd1306.h"
#include "rv3029.h"
#include "httpclient.h"

os_timer_t oled_refresh;
os_timer_t time_refresh;

bool scanning = false;

os_timer_t resume_timer;
void ICACHE_FLASH_ATTR resume_cb() {
	scanning = false;
}

void ICACHE_FLASH_ATTR scan_done(void *arg, STATUS status) {
	char ssid[33];
	ssd1306_fill(0x00);
	if (status == OK) {
		struct bss_info *bss_link = (struct bss_info *)arg;

		uint8_t i = 0;
		while (bss_link != NULL && i < 8) {
			os_memcpy(ssid, bss_link->ssid, os_strlen(bss_link->ssid));
			ssid[os_strlen(bss_link->ssid)] = 0x00;

			os_printf("> %s\r\n", ssid);
			ssd1306_text_small(0, i * 8, ssid);

			bss_link = bss_link->next.stqe_next;
			i++;
		}
	}

	os_timer_setfn(&resume_timer, (os_timer_func_t *)resume_cb, NULL);
	os_timer_arm(&resume_timer, 4000, 0);
	ssd1306_flip();
}

void ICACHE_FLASH_ATTR wifi_scan(void) {
	scanning = true;
	ssd1306_fill(0x00);
	ssd1306_text_large(20, 10, "Scanning...");
	ssd1306_flip();
	wifi_station_scan(NULL, scan_done);
}

void ICACHE_FLASH_ATTR oled_refresh_cb(void) {
	// Button status
	// if (!GPIO_INPUT_GET(4) && !GPIO_INPUT_GET(5)) wifi_scan();

	if (scanning) return;
	ssd1306_fill(0x00);

	struct TimeSpec time;
	rv3029_gettime(&time);
	char timestring[20];
	os_sprintf(timestring, "%2d:%02d:%02d", time.hours, time.minutes, time.seconds);
	ssd1306_text_large(32, 15, timestring);

	char datestring[20];
	os_sprintf(datestring, "%d-%02d-%02d", 2000 + time.years, time.months, time.date);
	ssd1306_text_small(34, 31, datestring);

	if (!GPIO_INPUT_GET(4)) {
		char voltline[10];
		float batt_volt = ((float)system_adc_read()) * (VOLT_DIV_DOWN + VOLT_DIV_UP) / VOLT_DIV_DOWN / 1024;
		os_sprintf(voltline, "%d.%02d V", (uint16_t)batt_volt, ((uint16_t)(batt_volt * 100)) % 100);
		ssd1306_text_small(46, 50, voltline);
	}

	ssd1306_flip();
}

void ICACHE_FLASH_ATTR http_callback_time(char *res, int status, char *res_full)
{
	if (status == HTTP_STATUS_GENERIC_ERROR) return;

	// Send "ESP" string in request to make sure the API actually answered the request
	if (res[0] == 'E' && res[1] == 'S' && res[2] == 'P') {
		// Split res by inserting NUL-termination characters between numbers
		uint8_t i = 0;
		while (res[i] != 0x00) {
			if (res[i] == ' ') res[i] = 0x00;
			i++;
		}

		struct TimeSpec settime;
		settime.years = atoi(res + 4);
		settime.months = atoi(res + 7);
		settime.date = atoi(res + 10);
		settime.dow = atoi(res + 13);
		settime.hours = atoi(res + 15);
		settime.minutes = atoi(res + 18);
		settime.seconds = atoi(res + 21);

		rv3029_settime(&settime);

		/*** WiFi sleep ***/
		wifi_station_disconnect();
		wifi_set_opmode(NULL_MODE);
		wifi_fpm_set_sleep_type(MODEM_SLEEP_T);
		wifi_fpm_open();
		wifi_fpm_do_sleep(0xfffffff);
	}
}

void ICACHE_FLASH_ATTR time_refresh_cb(void) {
	// year month date dow hours minutes seconds
	http_get("http://www.timeapi.org/cet?ESP%20\\y%20\\m%20\\d%20\\u%20\\H%20\\M%20\\S", "", http_callback_time);
}

void ICACHE_FLASH_ATTR user_init(void) {
	uart_div_modify(0, UART_CLK_FREQ / BAUD);

	// LCD Init
	i2c_master_gpio_init();
	i2c_scanbus();
	ssd1306_init(0);

	// ~Charge input
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);
	gpio_output_set(0, 0, 0, BIT13);

	// Button inputs
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
	gpio_output_set(0, 0, 0, BIT4);
	gpio_output_set(0, 0, 0, BIT5);
	PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO4_U);
	PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO5_U);

	// Setup Wifi
	wifi_set_opmode(STATION_MODE);

	// No need for a Password in the German "Freifunk" Network ;)
	char ssid[32] = "Freifunk";
	char pass[64] = "";
	struct station_config sta_conf;
	sta_conf.bssid_set = 0;   
    os_memcpy(&sta_conf.ssid, ssid, 32);
    os_memcpy(&sta_conf.password, pass, 64);

    wifi_station_set_config(&sta_conf);
	wifi_station_connect();

	//struct TimeSpec time;
	//rv3029_gettime(&time);
	//os_printf("Time: %d-%d-%d %d:%d:%d\r\n", 2000 + time.years, time.months, time.date, time.hours, time.minutes, time.seconds);

	os_printf("Startup\r\n");

	// Timers
	os_timer_setfn(&oled_refresh, (os_timer_func_t *)oled_refresh_cb, NULL);
	os_timer_arm(&oled_refresh, 1000, 1);

	os_timer_setfn(&time_refresh, (os_timer_func_t *)time_refresh_cb, NULL);
	os_timer_arm(&time_refresh, 120000, 1);
}
