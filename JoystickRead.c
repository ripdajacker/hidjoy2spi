#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <linux/joystick.h>
#include <fcntl.h>
#include <zconf.h>
#include "JoystickRead.h"

int fd;
static int running = 1;
pthread_t threadId;

void *readJoy(void *arg) {
    struct js_event e;
    void (*callback )(struct js_event *) = arg;
    size_t structSize = sizeof(e);
    while (running) {
        if (read(fd, &e, structSize) == structSize) {
            (*callback)(&e);
        }
    }
}

void startJoystickReadThread(char *file, void (*callback)(struct js_event *)) {
    fd = open(file, O_RDONLY);
    printf("Reading joystick: %s\n", file);
    if (fd == -1) {
        printf("Joystick file not found or permission denied\n");
        return;
    }

    // Don't join this one
    pthread_create(&threadId, NULL, readJoy, callback);
}

