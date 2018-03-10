#include <stdio.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include <pthread.h>
#include <stdlib.h>
#include "JoystickRead.h"
#include "SpiBus.h"
#include "Xpad.h"

pthread_mutex_t mutex;
pthread_mutexattr_t mutexAttr;

static XpadReport_Data_t lastReport;
static XpadRumble_t *lastRumble;


void onReadJoy(struct js_event *event) {
    printf("value: %d, type: %d, number:%d\n", event->type, event->value, event->number);

    pthread_mutex_lock(&mutex);
    convertEventToReport(&lastReport, event);
    pthread_mutex_unlock(&mutex);
}

XpadReport_Data_t getLastReport(void) {
    return lastReport;
}

void handleRumble(XpadRumble_t *rumble) {
    printf("Rumble! 0x%x 0x%x\n", rumble->left, rumble->right);
    lastRumble = rumble;
}

int main(int argc, char *argv[]) {
    printf("HID to SPI redirect - (c) 2018 Jesenko Mehmedbasic\n\n");
    if (argc < 2 || argc > 3) {
        printf("Wrong number of arguments, usage:\n");
        printf("hidspi <path_to_joystick> <path_to_spi> (optional <path_to_joystick_rumble>) \n");
        return -1;
    }

    pthread_mutexattr_init(&mutexAttr);
    pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &mutexAttr);

    startJoystickReadThread(argv[1], &onReadJoy);
    startSpiThread(argv[2], getLastReport, handleRumble);

    return 0;
}