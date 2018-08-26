#ifndef HID_SPI_REDIRECT_SPIBUS_H
#define HID_SPI_REDIRECT_SPIBUS_H

#include <stdint.h>
#include <linux/joystick.h>
#include "Xpad.h"


void startSpiThread(char *file, XpadReport_Data_t (*supplier)(void), void (*callback)(XpadRumble_t *));

void convertEventToPackedArray(uint8_t *dst, XpadReport_Data_t *src);

#endif