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

/*** Compass: ***/
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
