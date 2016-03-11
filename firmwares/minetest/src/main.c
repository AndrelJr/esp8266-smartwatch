// System
#include <user_interface.h>
#include <os_type.h>
#include <ets_sys.h>
#include <osapi.h>
#include <gpio.h>
#include <math.h>

// Configuration
#include "user_config.h"

// Libesphttpd
#include "webpages-espfs.h"
#include "httpdespfs.h"
#include "httpd.h"
#include "espfs.h"

// Project
#include "httpclient.h"
#include "i2c_master.h"
#include "buttons.h"
#include "mpu9250.h"
#include "ssd1306.h"
#include "rv3029.h"
#include "vector.h"
#include "util.h"

// Globals
os_timer_t sensor_timer;
os_timer_t setup_timer;
vec3f calib_bias;

vec3s16 acc;
vec3s16 gyro;
vec3s16 field;

void ICACHE_FLASH_ATTR sensor_timer_cb() {
	// Compass
	ak8963_get_field(&field);
	field.x -= calib_bias.x;
	field.y -= calib_bias.y;
	field.z -= calib_bias.z;
	//angles2f north = vec3s16_to_angles2f(field);
	ak8963_set_opmode(AK8963_SINGLE, AK8963_16BIT);

	// Acceleration
	mpu9250_acc_get_acceleration(&acc);

	// Gyro
	mpu9250_gyr_get_angvelocity(&gyro);

	//compass_yaw = north.yaw;
	//acc_pitch = atan2(acc.y, acc.z);
}

void ICACHE_FLASH_ATTR on_ak8963_calibration(vec3f bias) {
	calib_bias.x = bias.x;
	calib_bias.y = bias.y;
	calib_bias.z = bias.z;

	os_timer_setfn(&sensor_timer, (os_timer_func_t *)sensor_timer_cb, NULL);
	os_timer_arm(&sensor_timer, 50, true);
}

void ICACHE_FLASH_ATTR setup_timer_cb() {
	ak8963_calibration_start(10000, on_ak8963_calibration);
}

int cmd_minetest(HttpdConnData *conn) {
	char answer[200];

	int len = os_sprintf(answer,
			"{ \"acc\" : { \"x\" : %d, \"y\" : %d, \"z\" : %d }, \
			\"gyro\" : { \"x\" : %d, \"y\" : %d, \"z\" : %d }, \
			\"compass\" : { \"x\" : %d, \"y\" : %d, \"z\" : %d } }",
			acc.x, acc.y, acc.z, gyro.x, gyro.y, gyro.z, field.x, field.y, field.z);

	httpdStartResponse(conn, 200);
	httpdHeader(conn, "Content-Type", "application/json");
	httpdEndHeaders(conn);
	httpdSend(conn, answer, len);

	return HTTPD_CGI_DONE;
}

HttpdBuiltInUrl builtInUrls[]={
	{"/", cgiRedirect, "/index.html"},
	{"/minetest", cmd_minetest, NULL},
	{"*", cgiEspFsHook, NULL},
	{NULL, NULL, NULL}
};

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
	ssd1306_text_small(10, 10, "Minetest Mode!");
	ssd1306_flip();

	// ~Charge input
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);
	gpio_output_set(0, 0, 0, BIT13);

	// Button inputs
	buttons_init();

	/*** WiFi configuration ***/
	wifi_set_opmode(STATION_MODE);

	// This is just some sample WiFi network I created
	// for this purpose
	char ssid[32] = "minetest";
	char pass[64] = "minetest_handgelenk";
	struct station_config sta_conf;
	sta_conf.bssid_set = 0;
	os_memcpy(&sta_conf.ssid, ssid, 32);
	os_memcpy(&sta_conf.password, pass, 64);
	wifi_station_set_config(&sta_conf);
	wifi_station_connect();

	/*** Network configuration ***/
	wifi_station_dhcpc_stop();
	struct ip_info ip_conf;

	IP4_ADDR(&ip_conf.ip, 192, 168, 0, 60);
	IP4_ADDR(&ip_conf.gw, 192, 168, 0, 2);
	IP4_ADDR(&ip_conf.netmask, 255, 255, 0, 0);
	wifi_set_ip_info(STATION_IF, &ip_conf);

	/*** Compass calibration and setup ***/
	// Delay so that WiFi configuration can be applied before
	os_timer_setfn(&setup_timer, (os_timer_func_t *)setup_timer_cb, NULL);
	os_timer_arm(&setup_timer, 3000, false);

	/*** Accelerometer setup ***/
	mpu9250_acc_config(MPU9250_ACC_SCALE_8G);

	/*** Gyroscope setup ***/
	mpu9250_gyr_config(MPU9250_GYR_SCALE_250DPS);

	/*** Set up HTTP Server ***/
	espFsInit((void*)(webpages_espfs_start));
	httpdInit(builtInUrls, 80);

	os_printf("Startup\r\n");
}
