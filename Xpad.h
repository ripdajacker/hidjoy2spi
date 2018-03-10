//
// Created by jesenko on 3/9/18.
//

#ifndef HID_SPI_REDIRECT_XPAD_H
#define HID_SPI_REDIRECT_XPAD_H

#include <unitypes.h>

typedef struct {
    uint8_t UNUSED_0;
    uint8_t LENGTH; // always 0x14

    uint8_t DPAD_UP      :1;
    uint8_t DPAD_DOWN    :1;
    uint8_t DPAD_LEFT    :1;
    uint8_t DPAD_RIGHT   :1;

    uint8_t BUTTON_START :1;
    uint8_t BUTTON_BACK  :1;
    uint8_t BUTTON_L3    :1;
    uint8_t BUTTON_R3    :1;

    uint8_t UNUSED_1;

    uint8_t BUTTON_A;
    uint8_t BUTTON_B;
    uint8_t BUTTON_X;
    uint8_t BUTTON_Y;

    uint8_t BUTTON_BLACK;
    uint8_t BUTTON_WHITE;

    uint8_t TRIGGER_LEFT;
    uint8_t TRIGGER_RIGHT;

    int16_t LEFT_STICK_X;
    int16_t LEFT_STICK_Y;
    int16_t RIGHT_STICK_X;
    int16_t RIGHT_STICK_Y;

} XpadReport_Data_t;

typedef struct {
    uint8_t unused_0;
    uint8_t unused_len;
    uint8_t unused_1;

    uint8_t left;
    uint8_t unused_separator;
    uint8_t right;

    uint8_t padding[14];
} XpadRumble_t;

void convertEventToReport(XpadReport_Data_t *dst, struct js_event *event);

#endif //HID_SPI_REDIRECT_XPAD_H
