#include <linux/joystick.h>
#include <stdint.h>
#include "Xpad.h"

#define SRC_ORIGINAL_XPAD 0
#define SRC_XBOX_360      1

uint8_t sourceType = SRC_XBOX_360;

uint8_t map(signed short x) {
    long in_min = -32767;
    long in_max = 32767;
    long out_max = 255;
    return (uint8_t) ((x - in_min) * (out_max) / (in_max - in_min));
}

void handle360(XpadReport_Data_t *dst, struct js_event *event) {
    switch (event->type) {
        case JS_EVENT_BUTTON: {
            uint8_t button = (uint8_t) (event->value ? 0xFF : 0);
            uint8_t buttonDigital = (uint8_t) (event->value ? 1 : 0);
            switch (event->number) {
                case 0:
                    dst->BUTTON_A = button;
                    break;
                case 1:
                    dst->BUTTON_B = button;
                    break;
                case 4:
                    dst->BUTTON_BLACK = button;
                    break;
                case 2:
                    dst->BUTTON_X = button;
                    break;
                case 3:
                    dst->BUTTON_Y = button;
                    break;
                case 5:
                    dst->BUTTON_WHITE = button;
                    break;
                case 6:
                    dst->BUTTON_BACK = buttonDigital;
                    break;
                case 7:
                    dst->BUTTON_START = buttonDigital;
                    break;
                case 8:
                    dst->UNUSED_1 = buttonDigital;
                    break;
                case 9:
                    dst->BUTTON_L3 = buttonDigital;
                    break;
                case 10:
                    dst->BUTTON_R3 = buttonDigital;
                    break;
                default:
                    break;
            }
            break;
        }
        case JS_EVENT_AXIS: {
            switch (event->number) {
                case 0:
                    dst->LEFT_STICK_X = event->value;
                    break;
                case 1:
                    dst->LEFT_STICK_Y = -event->value;
                    break;
                case 2:
                    dst->TRIGGER_LEFT = map(event->value);
                    break;
                case 3:
                    dst->RIGHT_STICK_X = event->value;
                    break;
                case 4:
                    dst->RIGHT_STICK_Y = -event->value;
                    break;
                case 5:
                    dst->TRIGGER_RIGHT = map(event->value);
                    break;
                case 6:
                    dst->DPAD_LEFT = (uint8_t) (event->value == -32767 ? 1 : 0);
                    dst->DPAD_RIGHT = (uint8_t) (event->value == 32767 ? 1 : 0);
                    break;
                case 7:
                    dst->DPAD_UP = (uint8_t) (event->value == -32767 ? 1 : 0);
                    dst->DPAD_DOWN = (uint8_t) (event->value == 32767 ? 1 : 0);
                    break;
                default:
                    break;
            }
            break;
        }

        default:
            return;
    }
}

void handleOriginalXbox(XpadReport_Data_t *dst, struct js_event *event) {
    switch (event->type) {
        case JS_EVENT_BUTTON: {
            uint8_t button = (uint8_t) (event->value ? 0xFF : 0);
            switch (event->number) {
                case 0:
                    dst->BUTTON_A = button;
                    break;
                case 1:
                    dst->BUTTON_B = button;
                    break;
                case 2:
                    dst->BUTTON_BLACK = button;
                    break;
                case 3:
                    dst->BUTTON_X = button;
                    break;
                case 4:
                    dst->BUTTON_Y = button;
                    break;
                case 5:
                    dst->BUTTON_WHITE = button;
                    break;
                default:
                    break;
            }
            break;
        }
        case JS_EVENT_AXIS: {
            switch (event->number) {
                case 0:
                    dst->LEFT_STICK_Y = event->value;
                    break;
                case 1:
                    dst->LEFT_STICK_X = event->value;
                    break;
                case 2:
                    dst->TRIGGER_LEFT = map(event->value);
                    break;
                case 3:
                    dst->RIGHT_STICK_Y = event->value;
                    break;
                case 4:
                    dst->RIGHT_STICK_X = event->value;
                    break;
                case 5:
                    dst->TRIGGER_RIGHT = map(event->value);
                    break;
                case 6:
                    dst->DPAD_LEFT = (uint8_t) (event->value == -32767 ? 1 : 0);
                    dst->DPAD_RIGHT = (uint8_t) (event->value == 32767 ? 1 : 0);
                    break;
                case 7:
                    dst->DPAD_UP = (uint8_t) (event->value == -32767 ? 1 : 0);
                    dst->DPAD_DOWN = (uint8_t) (event->value == 32767 ? 1 : 0);
                    break;
                default:
                    break;
            }
            break;
        }

        default:
            return;
    }
}

void convertEventToReport(XpadReport_Data_t *dst, struct js_event *event) {
    if (sourceType == SRC_ORIGINAL_XPAD) {
        handleOriginalXbox(dst, event);
    } else if (sourceType == SRC_XBOX_360) {
        handle360(dst, event);
    }
}
