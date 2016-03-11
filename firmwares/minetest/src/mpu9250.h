#include <ets_sys.h>

#include "vector.h"

#ifndef _MPU9250_H
#define _MPU9250_H

// Configuration - Addresses include write/read bit
#define MPU9250_ADDR 0x68
#define AK8963_ADDR 0x0c
#define AK8963_CALIB_INTERVAL 10

/*** Common ***/
void mpu9250_init();
void mpu9250_i2cbypass_set(bool active);

/*** Accelerometer ***/
enum mpu9250_acc_scale {
	MPU9250_ACC_SCALE_2G = 0b00,
	MPU9250_ACC_SCALE_4G = 0b01,
	MPU9250_ACC_SCALE_8G = 0b10,
	MPU9250_ACC_SCALE_16G = 0b11,
};

bool mpu9250_acc_config(enum mpu9250_acc_scale scale);
bool mpu9250_acc_get_acceleration(vec3s16 *measurement);

/*** Gyroscope ***/
enum mpu9250_gyr_scale {
	MPU9250_GYR_SCALE_250DPS = 0b00,
	MPU9250_GYR_SCALE_500DPS = 0b01,
	MPU9250_GYR_SCALE_1000DPS = 0b10,
	MPU9250_GYR_SCALE_2000DPS = 0b11,
};

bool mpu9250_gyr_config(enum mpu9250_gyr_scale scale);
bool mpu9250_gyr_get_angvelocity(vec3s16 *measurement);

/*** Compass ***/
enum ak8963_opmode {
	AK8963_PWRDOWN = 0,
	AK8963_SINGLE = 1,
	AK8963_8HZ = 2,
	AK8963_100HZ = 6,
	AK8963_SELFTEST = 8,
	AK8963_FUSEACCESS = 15
};

enum ak8963_bitwidth {
	AK8963_14BIT = 0,
	AK8963_16BIT = (1<<4)
};

typedef void (*ak8963_calibration_callback)(vec3f bias);

bool ak8963_init();
bool ak8963_set_opmode(enum ak8963_opmode mode, enum ak8963_bitwidth width);
bool ak8963_get_drdy(bool *drdy);
bool ak8963_get_field(vec3s16 *measurement);

#endif
