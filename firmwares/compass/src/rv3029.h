#include <ets_sys.h>

#ifndef _RV3029_H
#define _RV3029_H

// Configuration
#define RV3029_ADDR_W 0xAC
#define RV3029_ADDR_R 0xAD

// 24-hour format only
// Years from 0 - 79, meaning 20xx
// Months from 1 - 12, dow from 1 - 7
struct TimeSpec {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t date;
	uint8_t dow;
	uint8_t months;
	uint8_t years;
};

int8_t rv3029_temp();
bool rv3029_gettime(struct TimeSpec *time);

// Internal
bool rv3029_read(uint8_t regaddr, uint8_t len, uint8_t *val);
bool rv3029_write(uint8_t regaddr, uint8_t len, uint8_t *val);

#endif
