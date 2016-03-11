#ifndef _SSD1306_H
#define	_SSD1306_H

/** Internal commands **/
void ssd1306_cmd(uint8_t cmd);
void ssd1306_data(uint8_t dat);
void ssd1306_databurst(uint8_t *dat, uint32_t len);
void ssd1306_cmdburst(uint8_t *dat, uint32_t len);

/** Initialization **/
void ssd1306_init(uint8_t contrast);

/** Text ouput **/
void ssd1306_text_small(uint8_t x, uint8_t y, char *text);
void ssd1306_text_large(uint8_t x, uint8_t y, char *text);

/** Buffer related **/
void ssd1306_fill(uint8_t content);
void ssd1306_flip(void);

/** Addressing modes **/
enum ssd1306_addr_mode {
	SSD1306_HORIZONTAL_ADDRESSING = 0,
	SSD1306_VERTICAL_ADDRESSING = 1,
	SSD1306_PAGE_ADDRESSING = 2
};

/** Scroll directions and frequency **/
enum ssd1306_scroll_dir {
	SSD1306_SCROLL_RIGHT = 0,
	SSD1306_SCROLL_LEFT = 1
};

enum ssd1306_scroll_frequency {
	SSD1306_SCROLLFREQ_5 = 0b000,
	SSD1306_SCROLLFREQ_64 = 0b001,
	SSD1306_SCROLLFREQ_128 = 0b010,
	SSD1306_SCROLLFREQ_256 = 0b011,
	SSD1306_SCROLLFREQ_3 = 0b100,
	SSD1306_SCROLLFREQ_4 = 0b101,
	SSD1306_SCROLLFREQ_25 = 0b110,
	SSD1306_SCROLLFREQ_2 = 0b111
};

/** Blinking / fading modes **/
enum ssd1306_blinkfade_mode {
	SSD1306_BLINKFADE_OFF = 0b00,
	SSD1306_BLINKFADE_FADE = 0b10,
	SSD1306_BLINKFADE_BLINK = 0b11
};

/** Misc commands **/
void ssd1306_setcontrast(uint8_t contrast);
void ssd1306_setonoff(bool on);
void ssd1306_setaddressing(enum ssd1306_addr_mode mode);
void ssd1306_setstartline(uint8_t startline);
void ssd1306_setstartpage(uint8_t startpage);
void ssd1306_setstartaddress(uint8_t address);
void ssd1306_setmirror_h(bool mirror);
void ssd1306_setmirror_v(bool mirror);
void ssd1306_setinverse(bool inverse);
void ssd1306_setoffset(uint8_t x);
void ssd1306_setosc(uint8_t divratio, uint8_t fosc);
void ssd1306_setprecharge(uint8_t period_phase1, uint8_t period_phase2);
void ssd1306_setcomconfig(uint8_t comconfig);
void ssd1306_setvcomh(uint8_t level);
void ssd1306_setchargepump(bool enable);
void ssd1306_setmultiplex(uint8_t multiplex);

/** Fading and Blinking **/
void ssd1306_setglow(bool enable);
void ssd1306_setblinkfade(enum ssd1306_blinkfade_mode mode, uint8_t time);

/** Scrolling **/
void ssd1306_scroll(enum ssd1306_scroll_dir direction, uint8_t startpage, uint8_t endpage,
		enum ssd1306_scroll_frequency freq, uint8_t voffset);
void ssd1306_vscroll_area(uint8_t fixedrows, uint8_t scrollrows);
void ssd1306_scroll_disable();

#endif
