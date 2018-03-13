#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <poll.h>
#include "SpiBus.h"
#include "spi.h"

static int fd = -1;
static int gpioFd = -1;
static int running = 1;
static XpadRumble_t rumble;

typedef struct {
    XpadReport_Data_t (*supplier)(void);

    void (*callback)(XpadRumble_t *);
} t_supplier_and_callback;

void initGpio(void) {
    gpioFd = open("/sys/class/gpio/export", O_WRONLY);
    write(gpioFd, "in", 2);
    close(gpioFd);

    gpioFd = open("/sys/class/gpio/gpio25/direction", O_WRONLY);
    write(gpioFd, "in", 2);
    close(gpioFd);

    gpioFd = open("/sys/class/gpio/gpio25/edge", O_WRONLY);
    write(gpioFd, "both", 4);
    close(gpioFd);

    gpioFd = open("/sys/class/gpio/gpio25/value", O_RDONLY);
}

void *readWriteSpi(void *args) {
    t_supplier_and_callback *holder = args;

    spi_init(fd);

    initGpio();
    struct pollfd polling;
    polling.fd = gpioFd;
    polling.events = POLLPRI;

    while (running) {
        lseek(gpioFd, 0, SEEK_SET);
        int gpioValue = poll(&polling, 1, 10);

        char c;
        read(gpioFd, &c, 1);

        if (gpioValue > 0) {
            if (c == '1') {
                XpadReport_Data_t reportData = (holder->supplier)();
                transfer(fd, (uint8_t *) &reportData, (uint8_t *) &rumble);

                (holder->callback)(&rumble);
            }
        }
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
