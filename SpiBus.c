#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <poll.h>
#include "SpiBus.h"
#include "spi.h"
#include "Xpad.h"

static int fd = -1;
static int running = 1;
static XpadRumble_t rumble;
uint8_t tx[20];

typedef struct {
    XpadReport_Data_t (*supplier)(void);

    void (*callback)(XpadRumble_t *);
} t_supplier_and_callback;

void *readWriteSpi(void *args) {
    t_supplier_and_callback *holder = args;

    spi_init(fd);

    while (running) {
        XpadReport_Data_t reportData = (holder->supplier)();

        convertEventToPackedArray((uint8_t *) &tx, &reportData);
        transfer(fd, (uint8_t *) &tx, (uint8_t *) &rumble);

        (holder->callback)(&rumble);
        usleep(1500);
    }
}


void startSpiThread(char *file, XpadReport_Data_t (*supplier)(void), void (*callback)(XpadRumble_t *)) {
    pthread_t threadId;
    fd = open(file, O_RDWR);

    printf("\nOpening SPI: %s\n", file);
    if (fd == -1) {
        printf("SPI file not found or permission denied\n");
        return;
    }

    t_supplier_and_callback arg;
    arg.callback = callback;
    arg.supplier = supplier;

    pthread_create(&threadId, NULL, readWriteSpi, &arg);
    pthread_join(threadId, NULL);
}


inline uint8_t mapBitMask(uint8_t input, uint8_t shift) {
    uint8_t value = input ? (uint8_t) 1 : (uint8_t) 0;
    return value << shift;
}

void convertEventToPackedArray(uint8_t *dst, XpadReport_Data_t *src) {
    dst[0] = 0;
    dst[1] = 0x14;

    dst[2] = (uint8_t) (mapBitMask(src->DPAD_UP, 0)
                        | mapBitMask(src->DPAD_DOWN, 1)
                        | mapBitMask(src->DPAD_LEFT, 2)
                        | mapBitMask(src->DPAD_RIGHT, 3)
                        | mapBitMask(src->BUTTON_START, 4)
                        | mapBitMask(src->BUTTON_BACK, 5)
                        | mapBitMask(src->BUTTON_L3, 6)
                        | mapBitMask(src->BUTTON_R3, 7));

    dst[3] = 0;

    dst[4] = src->BUTTON_A;
    dst[5] = src->BUTTON_B;
    dst[6] = src->BUTTON_X;
    dst[7] = src->BUTTON_Y;

    dst[8] = src->BUTTON_BLACK;
    dst[9] = src->BUTTON_WHITE;

    dst[10] = src->TRIGGER_LEFT;
    dst[11] = src->TRIGGER_RIGHT;

    dst[12] = (uint8_t) (src->LEFT_STICK_X & 0x00FF);
    dst[13] = (uint8_t) (src->LEFT_STICK_X & 0xFF00);

    dst[14] = (uint8_t) (src->LEFT_STICK_Y & 0x00FF);
    dst[15] = (uint8_t) (src->LEFT_STICK_Y & 0xFF00);

    dst[16] = (uint8_t) (src->RIGHT_STICK_X & 0x00FF);
    dst[17] = (uint8_t) (src->RIGHT_STICK_X & 0xFF00);

    dst[18] = (uint8_t) (src->RIGHT_STICK_Y & 0x00FF);
    dst[19] = (uint8_t) (src->RIGHT_STICK_Y & 0xFF00);
}