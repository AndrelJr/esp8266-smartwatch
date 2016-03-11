#include "ets_sys.h"
#include "osapi.h"

#include "i2c_master.h"
#include "rv3029.h"

#define RV3029_ADDR_W 0xAC
#define RV3029_ADDR_R 0xAD

#define BCD2BIN(val) ((((val) & 0x0f) + ((val) >> 4) * 10))
#define BIN2BCD(val) ((((val)/10)<<4) + (val)%10)

bool rv3029_read(uint8_t regaddr, uint8_t bytes, uint8_t *val) {
    i2c_master_start();

	// Write I²C Address for writing register
    i2c_master_writeByte(RV3029_ADDR_W);
    if (i2c_master_getAck()) {
		os_printf("RV3029: no ACK for write addr\r\n");
		i2c_master_stop();
		return false;
	}

	// Write register address
    i2c_master_writeByte(regaddr);
    if(i2c_master_getAck()) {
		os_printf("RV3029: no ACK for register\r\n");
		i2c_master_stop();
		return false;
	}

	i2c_master_stop();
	i2c_master_start();

	// Write I²C Address for reading
    i2c_master_writeByte(RV3029_ADDR_R);
    if (i2c_master_getAck()) {
		os_printf("RV3029: no ACK for read addr\r\n");
		i2c_master_stop();
		return false;
	}

	uint8_t i;
	for (i = 0; i < bytes; ++i) {
		val[i] =  i2c_master_readByte();
		i2c_master_send_ack();
	}

	i2c_master_stop();

	return true;
}

bool rv3029_write(uint8_t regaddr, uint8_t bytes, uint8_t *val) {
    i2c_master_start();

	// Write I²C Address for writing register
    i2c_master_writeByte(RV3029_ADDR_W);
    if (i2c_master_getAck()) {
		os_printf("RV3029: no ACK for write addr\r\n");
		i2c_master_stop();
		return false;
	}

	// Write register address
    i2c_master_writeByte(regaddr);
    if(i2c_master_getAck()) {
		os_printf("RV3029: no ACK for register\r\n");
		i2c_master_stop();
		return false;
	}

	uint8_t i;
	for (i = 0; i < bytes; ++i) {
		i2c_master_writeByte(val[i]);
	    if(i2c_master_getAck()) {
			os_printf("RV3029: no ACK for datawrite\r\n");
			i2c_master_stop();
			return false;
		}
	}

	i2c_master_stop();

	return true;
}


int8_t rv3029_temp() {
	uint8_t temp;
	if (rv3029_read(0x20, 1, &temp)) return (int8_t)temp - 60;
	else return 255;
}

bool rv3029_gettime(struct TimeSpec *time) {
	uint8_t raw[7];
	if (!rv3029_read(0x08, 7, raw)) return false;

	time->seconds = BCD2BIN(raw[0]);
	time->minutes = BCD2BIN(raw[1]);
	time->hours = BCD2BIN(raw[2]);
	time->date = BCD2BIN(raw[3]);
	time->dow = raw[4];
	time->months = BCD2BIN(raw[5]);
	time->years = BCD2BIN(raw[6]);

	return true;
}

bool rv3029_settime(struct TimeSpec *time) {
	uint8_t raw[7];
	raw[0] = BIN2BCD(time->seconds);
	raw[1] = BIN2BCD(time->minutes);
	raw[2] = BIN2BCD(time->hours);
	raw[3] = BIN2BCD(time->date);
	raw[4] = time->dow;
	raw[5] = BIN2BCD(time->months);
	raw[6] = BIN2BCD(time->years);
	return rv3029_write(0x08, 7, raw);
}
