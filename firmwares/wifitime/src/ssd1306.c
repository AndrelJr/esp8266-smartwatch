#include <ets_sys.h>
#include <osapi.h>
#include <os_type.h>

#include "user_config.h"
#include "ssd1306.h"
#include "i2c_master.h"
#include "font.h"

#define CONTROL_CMD		0b00000000
#define CONTROL_DATA	0b01000000
#define SSD1306_PAGES	8
#define SSD1306_WIDTH	128
#define SSD1306_ADDR	0x78

uint8_t buf[SSD1306_PAGES][SSD1306_WIDTH];
bool buf_changed[SSD1306_PAGES] = { false };

/** Internal commands **/
void ssd1306_putbyte(uint8_t byte) {
	i2c_master_writeByte(byte);
    if (i2c_master_getAck()) os_printf("SSD1306: No ACK for 0x%02x\r\n", byte);
}

void ICACHE_FLASH_ATTR ssd1306_cmd(uint8_t cmd) {
	i2c_master_start();
	ssd1306_putbyte(SSD1306_ADDR);
	ssd1306_putbyte(CONTROL_CMD);
	ssd1306_putbyte(cmd);
	i2c_master_stop();
}

void ICACHE_FLASH_ATTR ssd1306_data(uint8_t dat) {
	i2c_master_start();
	ssd1306_putbyte(SSD1306_ADDR);
	ssd1306_putbyte(CONTROL_DATA);
	ssd1306_putbyte(dat);
	i2c_master_stop();
}

void ICACHE_FLASH_ATTR ssd1306_databurst(uint8_t *dat, uint32_t len) {
	i2c_master_start();
	ssd1306_putbyte(SSD1306_ADDR);
	ssd1306_putbyte(CONTROL_DATA);
	uint32_t i;
	for (i = 0; i < len; ++i) ssd1306_putbyte(dat[i]);
	i2c_master_stop();
}

void ICACHE_FLASH_ATTR ssd1306_cmdburst(uint8_t *dat, uint32_t len) {
	i2c_master_start();
	ssd1306_putbyte(SSD1306_ADDR);
	ssd1306_putbyte(CONTROL_CMD);
	uint16_t i;
	for (i = 0; i < len; ++i) ssd1306_putbyte(dat[i]);
	i2c_master_stop();
}

/** Initialization **/
void ICACHE_FLASH_ATTR ssd1306_init(uint8_t contrast) {
	os_delay_us(10000);

	ssd1306_setaddressing(SSD1306_PAGE_ADDRESSING);
	ssd1306_setstartline(0);
	ssd1306_setstartpage(0);
	ssd1306_setstartaddress(0);
	ssd1306_setcontrast(contrast);
	ssd1306_setglow(0);
	ssd1306_setmirror_h(false);
	ssd1306_setinverse(false);
	ssd1306_setmultiplex(63);
	ssd1306_setmirror_v(false);
	ssd1306_setoffset(0);
	ssd1306_setosc(0, 8);
	ssd1306_setprecharge(2, 2);
	ssd1306_setcomconfig(0x12);
	ssd1306_setvcomh(0x40);
	ssd1306_scroll_disable();
	ssd1306_vscroll_area(0, 64);
	ssd1306_setblinkfade(SSD1306_BLINKFADE_OFF, 0);
	ssd1306_setchargepump(true);
	ssd1306_setonoff(true);
}

/** Text ouput **/
void ssd1306_text_small(uint8_t x, uint8_t y, char *text) {
	uint16_t c = 0;
	uint8_t page = y / 8;
	while (text[c] != 0x00) {
		uint8_t i;
		for (i = 0; i < 6; ++i) {
			uint16_t xpos = x + c * 6 + i;
			if (xpos >= SSD1306_WIDTH) return;
			uint8_t draw = font_small[text[c] - 0x20][i];

			// The text may now span between two pages, so the bits have
			// to be sperated for first and second page
			uint8_t firstpage = (draw << y % 8);
			uint8_t secondpage = (draw >> (8 - y % 8));

			buf[page][xpos] |= firstpage;
			buf_changed[page] = true;
			if (y % 8) {
				buf_changed[page + 1] = true;
				buf[page + 1][xpos] |= secondpage;
			}
		}
		c++;
	}
}

void ssd1306_text_large(uint8_t x, uint8_t y, char *text) {
	uint16_t c = 0;
	uint8_t page = y / 8;
	while (text[c] != 0x00) {
		uint8_t i;
		for (i = 0; i < 8; ++i) {
			uint16_t xpos = x + c * 8 + i;
			if (xpos >= SSD1306_WIDTH) return;
			uint16_t draw_firstpage = font_large[text[c] - 0x20][i];
			uint16_t draw_secondpage = font_large[text[c] - 0x20][i+8];

			// The text may now span between two pages, so the bits have
			// to be sperated for first and second page
			uint8_t firstpage = draw_firstpage << (y % 8);
			uint8_t secondpage = (draw_firstpage >> (8 - y % 8)) | (draw_secondpage << (y % 8));
			uint8_t thirdpage = draw_secondpage >> (8 - y % 8);

			buf_changed[page] = true;
			buf_changed[page + 1] = true;

			buf[page][xpos] |= firstpage;
			buf[page + 1][xpos] |= secondpage;

			if (y % 8) {
				buf[page + 2][xpos] |= thirdpage;
				buf_changed[page + 2] = true;
			}
		}
		c++;
	}
}

/** Buffer related **/
void ICACHE_FLASH_ATTR ssd1306_fill(uint8_t content) {
	uint8_t page = 0, col = 0;
	for(page = 0; page < 8; page++) {
		buf_changed[page] = true;
		for(col = 0; col < SSD1306_WIDTH; col++)
			buf[page][col] = content;
	}
}

// Requires page addressing mode
void ICACHE_FLASH_ATTR ssd1306_flip(void) {
	// Some dummy commands, to make controller align timing for bursts
	ssd1306_setglow(false);
	ssd1306_scroll_disable();

	uint8_t page;
	for (page = 0; page < SSD1306_PAGES; page++) {
		ssd1306_setstartaddress(0);
		ssd1306_setstartpage(page);
		ssd1306_databurst(buf[page], 128);
	}
}

/** Misc commands **/
void ICACHE_FLASH_ATTR ssd1306_setcontrast(uint8_t contrast) {
	ssd1306_cmd(0x81);
	ssd1306_cmd(contrast);
}

void ICACHE_FLASH_ATTR ssd1306_setonoff(bool on) {
	ssd1306_cmd(0xAE + (on ? 1 : 0));
}

void ICACHE_FLASH_ATTR ssd1306_setaddressing(enum ssd1306_addr_mode mode) {
	ssd1306_cmd(0x20);
	ssd1306_cmd(mode);
}

// startline from 0 - 63
void ICACHE_FLASH_ATTR ssd1306_setstartline(uint8_t startline) {
	ssd1306_cmd(0x40 + startline);
}

// startpage from 0 - 7
void ICACHE_FLASH_ATTR ssd1306_setstartpage(uint8_t startpage) {
	ssd1306_cmd(0xB0 + startpage);
}

// start address from 0 - 128
void ICACHE_FLASH_ATTR ssd1306_setstartaddress(uint8_t address) {
	ssd1306_cmd((0x10 | (address & 0xf0) >> 4));
	ssd1306_cmd(0x01 | (address & 0x0f));
}

void ICACHE_FLASH_ATTR ssd1306_setmirror_h(bool mirror) {
	ssd1306_cmd(0xA0 + (mirror ? 1 : 0));
}

void ICACHE_FLASH_ATTR ssd1306_setmirror_v(bool mirror) {
	ssd1306_cmd(0xC0 + (mirror ? 8 : 0));
}

void ICACHE_FLASH_ATTR ssd1306_setinverse(bool inverse) {
	ssd1306_cmd(0xA6 + (inverse ? 1 : 0));
}

void ICACHE_FLASH_ATTR ssd1306_setoffset(uint8_t x) {
	ssd1306_cmd(0xD3);
	ssd1306_cmd(x);
}

// division ration from 0 - 15, fosc from 0 - 15, see datasheet section 8.3
// "Oscillator Circuit and Display Time Generator"
void ICACHE_FLASH_ATTR ssd1306_setosc(uint8_t divratio, uint8_t fosc) {
	ssd1306_cmd(0xD5);
	ssd1306_cmd(divratio | (fosc << 4));
}

void ICACHE_FLASH_ATTR ssd1306_setprecharge(uint8_t period_phase1, uint8_t period_phase2) {
	ssd1306_cmd(0xD9);
	ssd1306_cmd(period_phase1 | (period_phase2 << 4));
}

// Refer to datasheet section 10.1.18 for more information
// configures connection of panel to controller
void ICACHE_FLASH_ATTR ssd1306_setcomconfig(uint8_t comconfig) {
	ssd1306_cmd(0xDA);
	ssd1306_cmd(comconfig);
}

// level is either 0x00 (0.65 * Vcc), 0x40 (0.77 * Vcc, default), 0x60 (0.83 * Vcc)
void ICACHE_FLASH_ATTR ssd1306_setvcomh(uint8_t level) {
	ssd1306_cmd(0xDB);
	ssd1306_cmd(level);
}

void ICACHE_FLASH_ATTR ssd1306_setchargepump(bool enable) {
	ssd1306_cmd(0x8D);
	ssd1306_cmd(enable ? 0x14 : 0x10);
}

// multiplex from 15- 63
void ICACHE_FLASH_ATTR ssd1306_setmultiplex(uint8_t multiplex) {
	ssd1306_cmd(0xA8);
	ssd1306_cmd(multiplex);
}

/** Fading and Blinking **/
// Turns all pixels on (or disables this feature)
void ICACHE_FLASH_ATTR ssd1306_setglow(bool enable) {
	ssd1306_cmd(0xA4 + (enable ? 1 : 0));
}

// Speed 0 - 15
void ICACHE_FLASH_ATTR ssd1306_setblinkfade(enum ssd1306_blinkfade_mode mode, uint8_t time) {
	ssd1306_cmd(0x23);
	ssd1306_cmd((mode << 4) | time);
}


/** Scrolling **/
void ICACHE_FLASH_ATTR ssd1306_scroll(enum ssd1306_scroll_dir direction, uint8_t startpage, uint8_t endpage,
		enum ssd1306_scroll_frequency freq, uint8_t voffset) {
	ssd1306_cmd(0x29 + direction);
	ssd1306_cmd(0x00);
	ssd1306_cmd(startpage);
	ssd1306_cmd(freq);
	ssd1306_cmd(endpage);
	ssd1306_cmd(voffset);
	ssd1306_cmd(0x2f);
}

void ICACHE_FLASH_ATTR ssd1306_vscroll_area(uint8_t fixedrows, uint8_t scrollrows) {
	ssd1306_cmd(0xa3);
	ssd1306_cmd(fixedrows);
	ssd1306_cmd(scrollrows);
}

void ICACHE_FLASH_ATTR ssd1306_scroll_disable() {
	ssd1306_cmd(0x2e);
}
