#include <stdio.h>

#ifndef HID_SPI_REDIRECT_JOYSTICKREAD_H
#define HID_SPI_REDIRECT_JOYSTICKREAD_H

void *readJoy(void *vararg);

void startJoystickReadThread(char *file, void (*callback)(struct js_event *));

#endif //HID_SPI_REDIRECT_JOYSTICKREAD_H
