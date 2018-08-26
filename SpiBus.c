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

typedef struct {
    XpadReport_Data_t (*supplier)(void);

    void (*callback)(XpadRumble_t *);
} t_supplier_and_callback;

void *readWriteSpi(void *args) {
    t_supplier_and_callback *holder = args;

    spi_init(fd);

    while (running) {
        XpadReport_Data_t reportData = (holder->supplier)();
        reportData.LENGTH = 0x14;
        transfer(fd, (uint8_t *) &reportData, (uint8_t *) &rumble);

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
