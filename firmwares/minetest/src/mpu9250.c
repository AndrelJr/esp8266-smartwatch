// System
#include <ets_sys.h>
#include <os_type.h>
#include <osapi.h>

// Project
#include "mpu9250_reg.h"
#include "i2c_master.h"
#include "mpu9250.h"
#include "vector.h"
#include "util.h"

/*** Internal I²C functions ***/
#define mpu9250_write(regaddr, len, data) (i2cdevice_write(MPU9250_ADDR, regaddr, len, data))
#define mpu9250_read(regaddr, len, data) (i2cdevice_read(MPU9250_ADDR, regaddr, len, data))
#define mpu9250_readbyte(regaddr, data) (i2cdevice_readbyte(MPU9250_ADDR, regaddr, data))
#define mpu9250_writebyte(regaddr, data) (i2cdevice_writebyte(MPU9250_ADDR, regaddr, data))
#define mpu9250_readbit(regaddr, bit, val) (i2cdevice_readbit(MPU9250_ADDR, regaddr, bit, val))
#define mpu9250_writebit(regaddr, bit, val) (i2cdevice_writebit(MPU9250_ADDR, regaddr, bit, val))

#define ak8963_write(regaddr, len, data) (i2cdevice_write(AK8963_ADDR, regaddr, len, data))
#define ak8963_read(regaddr, len, data) (i2cdevice_read(AK8963_ADDR, regaddr, len, data))
#define ak8963_readbyte(regaddr, data) (i2cdevice_readbyte(AK8963_ADDR, regaddr, data))
#define ak8963_writebyte(regaddr, data) (i2cdevice_writebyte(AK8963_ADDR, regaddr, data))
#define ak8963_readbit(regaddr, bit, val) (i2cdevice_readbit(AK8963_ADDR, regaddr, bit, val))
#define ak8963_writebit(regaddr, bit, val) (i2cdevice_writebit(AK8963_ADDR, regaddr, bit, val))

/*** Common ***/
/*
 * Initialize to configuration used in this project:
 * - Reset MPU-9250 and restore sane defaults
 * - Some dummy reads to ensure communication works
 * - Enable I²C Bypass to Compass
 */
void ICACHE_FLASH_ATTR mpu9250_init() {
	// Reset MPU-9250
	mpu9250_writebyte(MPU9250_RA_PWR_MGMT_1, 1<<MPU9250_PWR1_DEVICE_RESET_BIT);
	os_delay_us(10000);

	// Select clock source, enable accelerometer and gyrometer,
	mpu9250_writebyte(MPU9250_RA_PWR_MGMT_1, 0x01);
	mpu9250_writebyte(MPU9250_RA_PWR_MGMT_2, 0x00);

	// Dummy read:
	uint8_t id;
	mpu9250_read(MPU9250_RA_WHO_AM_I, 1, &id);
	os_printf("MPU9250 WHOAMI: %x\r\n", id);

	// I²C Bypass:
	mpu9250_i2cbypass_set(true);
	os_delay_us(1000);
}

void ICACHE_FLASH_ATTR mpu9250_i2cbypass_set(bool active) {
	mpu9250_writebit(MPU9250_RA_INT_PIN_CFG, MPU9250_INTCFG_I2C_BYPASS_EN_BIT, active);
}

/*** Accelerometer ***/
bool ICACHE_FLASH_ATTR mpu9250_acc_config(enum mpu9250_acc_scale scale) {
	if (!mpu9250_writebit(MPU9250_RA_ACCEL_CONFIG, 3, gbi(scale, 0))) return false;
	if (!mpu9250_writebit(MPU9250_RA_ACCEL_CONFIG, 4, gbi(scale, 1))) return false;

	return true;
}

bool ICACHE_FLASH_ATTR mpu9250_acc_get_acceleration(vec3s16 *measurement) {
	uint8_t buffer[6];
	if (!mpu9250_read(MPU9250_RA_ACCEL_XOUT_H, 6, buffer)) return false;
	measurement->x = (((int16_t)buffer[0]) << 8) | buffer[1];
	measurement->y = (((int16_t)buffer[2]) << 8) | buffer[3];
	measurement->z = (((int16_t)buffer[4]) << 8) | buffer[5];

	return true;
}

/*** Gyroscope ***/
bool ICACHE_FLASH_ATTR mpu9250_gyr_config(enum mpu9250_gyr_scale scale) {
	if (!mpu9250_writebit(MPU9250_RA_GYRO_CONFIG, 3, gbi(scale, 0))) return false;
	if (!mpu9250_writebit(MPU9250_RA_GYRO_CONFIG, 4, gbi(scale, 1))) return false;

	return true;
}

bool ICACHE_FLASH_ATTR mpu9250_gyr_get_angvelocity(vec3s16 *measurement) {
	uint8_t buffer[6];
	if (!mpu9250_read(MPU9250_RA_GYRO_XOUT_H, 6, buffer)) return false;
	measurement->x = (((int16_t)buffer[0]) << 8) | buffer[1];
	measurement->y = (((int16_t)buffer[2]) << 8) | buffer[3];
	measurement->z = (((int16_t)buffer[4]) << 8) | buffer[5];

	return true;
}

/*** Compass: ***/
bool ICACHE_FLASH_ATTR ak8963_init() {
	// Reset device
	if (!ak8963_writebyte(AK8963_RA_CNTL2, 1)) return false;
	os_delay_us(1000);

	uint8_t wia;
	ak8963_readbyte(AK8963_RA_WIA, &wia);
	if (!os_printf("AK8963 WIA: %x\r\n", wia)) return false;

	return true;
}

bool ICACHE_FLASH_ATTR ak8963_set_opmode(enum ak8963_opmode mode, enum ak8963_bitwidth width) {
	return ak8963_writebyte(AK8963_RA_CNTL1, mode | width);
}

bool ICACHE_FLASH_ATTR ak8963_get_drdy(bool *drdy) {
	return ak8963_readbit(AK8963_RA_ST1, 0, drdy);
}

bool ICACHE_FLASH_ATTR ak8963_get_field(vec3s16 *measurement) {
	uint8_t buffer[6];
	if (!ak8963_read(AK8963_RA_HXL, 6, buffer)) return false;
	measurement->x = (((int16_t)buffer[1]) << 8) | buffer[0];
	measurement->y = (((int16_t)buffer[3]) << 8) | buffer[2];
	measurement->z = (((int16_t)buffer[5]) << 8) | buffer[4];

	return true;
}

/*** Compass Calibration: ***/
struct ak8963_calib_proc {
	os_timer_t datatimer;
	os_timer_t completetimer;
	vec3s16 maximum;
	vec3s16 minimum;
	ak8963_calibration_callback complete;
};

void ICACHE_FLASH_ATTR ak8963_calibration_datatimer_cb(struct ak8963_calib_proc *proc) {
	bool drdy;
	ak8963_get_drdy(&drdy);
	if (!drdy) return;

	vec3s16 field;
	if (!ak8963_get_field(&field)) return;

	// Store maximum and minimum magnetic values
	if (field.x > proc->maximum.x) proc->maximum.x = field.x;
	if (field.y > proc->maximum.y) proc->maximum.y = field.y;
	if (field.z > proc->maximum.z) proc->maximum.z = field.z;

	if (field.x < proc->minimum.x) proc->minimum.x = field.x;
	if (field.y < proc->minimum.y) proc->minimum.y = field.y;
	if (field.z < proc->minimum.z) proc->minimum.z = field.z;

	ak8963_set_opmode(AK8963_SINGLE, AK8963_16BIT);
}

void ICACHE_FLASH_ATTR ak8963_calibration_completetimer_cb(struct ak8963_calib_proc *proc) {
	os_timer_disarm(proc->datatimer);

	os_printf("Maximum: %d, %d, %d\r\n", proc->maximum.x, proc->maximum.y, proc->maximum.z);
	os_printf("Minimum: %d, %d, %d\r\n", proc->minimum.x, proc->minimum.y, proc->minimum.z);

	vec3f bias;
	bias.x = (proc->maximum.x + proc->minimum.x) / 2.f;
	bias.y = (proc->maximum.y + proc->minimum.y) / 2.f;
	bias.z = (proc->maximum.z + proc->minimum.z) / 2.f;

	proc->complete(bias);
}

void ICACHE_FLASH_ATTR ak8963_calibration_start(uint16_t milliseconds, ak8963_calibration_callback cb) {
	static struct ak8963_calib_proc proc;

	proc.complete = cb;
	proc.maximum.x = 0x8000; proc.maximum.y = 0x8000; proc.maximum.z = 0x8000;
	proc.minimum.x = 0x7fff; proc.minimum.y = 0x7fff; proc.minimum.z = 0x7fff;

	os_timer_setfn(&proc.datatimer, (os_timer_func_t *)ak8963_calibration_datatimer_cb, &proc);
	os_timer_arm(&proc.datatimer, AK8963_CALIB_INTERVAL, true);

	os_timer_setfn(&proc.completetimer, (os_timer_func_t *)ak8963_calibration_completetimer_cb, &proc);
	os_timer_arm(&proc.completetimer, milliseconds, false);

	ak8963_set_opmode(AK8963_SINGLE, AK8963_16BIT);
}
