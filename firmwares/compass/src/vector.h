#include <ets_sys.h>

#ifndef _VECTOR_H
#define _VECTOR_H

struct _vec3s16_t {
	int16_t x;
	int16_t y;
	int16_t z;
};
typedef struct _vec3s16_t vec3s16;

struct _vec3f_t {
	float x;
	float y;
	float z;
};
typedef struct _vec3f_t vec3f;

struct _angles2f_t {
	float pitch;
	float yaw;
};
typedef struct _angles2f_t angles2f;

angles2f vec3f_to_angles2f(vec3f v);
angles2f vec3s16_to_angles2f(vec3s16 v);

#endif
