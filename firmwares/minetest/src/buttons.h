#ifndef _BUTTONS_H
#define _BUTTONS_H

enum ButtonIdentifier {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_CENTER
};
typedef enum ButtonIdentifier button_t;

void buttons_init();
bool buttons_get(button_t btn);

#endif
