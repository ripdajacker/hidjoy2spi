#ifndef HID_SPI_REDIRECT_JOYSTICKREAD_H
#define HID_SPI_REDIRECT_JOYSTICKREAD_H

#include <stdio.h>

void *readJoy(void *vararg);

void startJoystickReadThread(char *file, void (*callback)(struct js_event *));

#endif