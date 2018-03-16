#include "spi.h"

uint8_t spi_mode = 0;
uint8_t spi_bits = 8;
uint32_t spi_speed = 500000;

void pabort(const char *s) {
    perror(s);
    abort();
}

int spi_set_mode(int fd, uint8_t mode) {
    int ret;

    spi_mode = mode;
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1) {
        pabort("can't set spi mode");
    }

    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (ret == -1) {
        pabort("can't get spi mode");
    }
    return ret;
}

//this function sets word length (bits per word)
int spi_set_word(int fd, uint8_t bits) {
    int ret;
    /*
     * bits per word
     */
    spi_bits = bits;
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1) {
        pabort("can't set bits per word");
    }

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1) {
        pabort("can't get bits per word");
    }
    return ret;

}

//this function sets communication speed.
int spi_set_speed(int fd, uint32_t speed) {
    int ret = 0;
    /*
     * max speed hz
     */
    spi_speed = speed;
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1) {
        pabort("can't set max speed hz");
    }

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1) {
        pabort("can't get max speed hz");
    }
    return ret;
}


int transfer(int fd, uint8_t *data, uint8_t *receive) {
    int returnValue;
    uint8_t tx[SPI_BUFFER_SIZE];
    static uint8_t rx[SPI_BUFFER_SIZE] = {0,};


    for (int i = 0; i < SPI_BUFFER_SIZE; i++) {
        tx[i] = data[i];
    }

    struct spi_ioc_transfer tr = {
            .tx_buf = (unsigned long) tx,
            .rx_buf = (unsigned long) rx,
            .len = SPI_BUFFER_SIZE,
            .delay_usecs = 1,
            .speed_hz = spi_speed,
            .bits_per_word = spi_bits,
    };

    returnValue = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (returnValue < 1) {
        pabort("can't send spi message");
    }

    //put received data back in to the specified return buffer.
    for (int i = 0; i < SPI_BUFFER_SIZE; i++) {
        receive[i] = rx[i];
    }
    return returnValue;
}

void spi_init(int fd) {
    spi_set_mode(fd, spi_mode);
    spi_set_word(fd, spi_bits);
    spi_set_speed(fd, spi_speed);
}