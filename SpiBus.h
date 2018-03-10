#ifndef HID_SPI_REDIRECT_SPIBUS_H
#define HID_SPI_REDIRECT_SPIBUS_H

#include <unitypes.h>
#include <linux/joystick.h>
#include "Xpad.h"


void startSpiThread(char *file, XpadReport_Data_t (*supplier)(void), void (*callback)(XpadRumble_t *));

#endif //HID_SPI_REDIRECT_SPIBUS_H
