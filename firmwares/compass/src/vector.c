// System
#include <math.h>

// Project
#include "vector.h"

angles2f ICACHE_FLASH_ATTR vec3f_to_angles2f(vec3f v) {
	angles2f ret;
	ret.yaw = atan2(v.y, v.x);
	ret.pitch = atan2(v.z, sqrt((v.x * v.x) + (v.y * v.y)));
	return ret;
}

angles2f ICACHE_FLASH_ATTR vec3s16_to_angles2f(vec3s16 v) {
	angles2f ret;
	ret.yaw = atan2(v.y, v.x);
	ret.pitch = atan2(v.z, sqrt((v.x * v.x) + (v.y * v.y)));
	return ret;
}
