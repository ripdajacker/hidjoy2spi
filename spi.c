#include "spi.h"

//variables for a standerd connection. mode0,0 8 bits wide. transfer speed 500Khz
//static const char *spi_device = "/dev/spidev0.0";
uint8_t spi_mode = 0;
uint8_t spi_bits = 8;
uint32_t spi_speed = 500000;
uint16_t spi_delay = 0;


//prints error message and aborts. (exits)
void pabort(const char *s)
{
	perror(s);
	abort();
}

//this function sets spi mode
int spi_set_mode(int fd, int mode)
{
	int ret;
	/*
	 * spi mode
	 */
	spi_mode = mode;
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");
	return ret;
}
//this function sets word length (bits per word)
int spi_set_word(int fd, int bits)
{
	int ret;
	/*
	 * bits per word
	 */
	spi_bits = bits;
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");
	return ret;

}
//this function sets communication speed.
int spi_set_speed(int fd, int speed)
{
	int ret = 0;
	/*
	 * max speed hz
	 */
	spi_speed = speed;
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");
	return ret;
}
/*
 * how long to delay after the last bit transfer
 * before optionally deselecting the device before the next transfer.
 * */
void spi_set_delay(int delay)
{
	spi_delay = delay;
}

//send and recieve data.
int transfer(int fd, uint8_t *data, uint8_t *receive)
{
	int ret,i;
	uint8_t tx[SPI_BUFFER_SIZE];
	static uint8_t rx[SPI_BUFFER_SIZE] = {0, };
	//fill tx with data to be send.
	for(i = 0;i<SPI_BUFFER_SIZE;i++)
	{
		tx[i] = data[i];
	}
	struct spi_ioc_transfer tr = 
	{
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = SPI_BUFFER_SIZE,
		.delay_usecs = spi_delay,
		.speed_hz = spi_speed,
		.bits_per_word = spi_bits,
	};
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if(ret < 1)
		pabort("can't send spi message");
		
	#ifdef DEBUG
	printf("Received: ");
	for(i = 0;i<SPI_BUFFER_SIZE;i++)
		printf("%02X ", rx[i]);
	printf("\n");
	#endif
	//put received data back in to the specified return buffer.
	for(i = 0;i<SPI_BUFFER_SIZE;i++)
	{
		receive[i] = rx[i];
	}
	return ret;
}

//a loopback test. to see if your spi works 
#ifdef DEBUG
void loopbackTest(int fd)
{
	int ret;
	uint8_t tx[] = {
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0x40, 0x00, 0x00, 0x00, 0x00, 0x95,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xAD,
		0xF0, 0x0D,
	};
	uint8_t rx[ARRAY_SIZE(tx)] = {0, };
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = ARRAY_SIZE(tx),
		.delay_usecs = spi_delay,
		.speed_hz = spi_speed,
		.bits_per_word = spi_bits,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");

	for (ret = 0; ret < ARRAY_SIZE(tx); ret++) {
		if (!(ret % 6))
			puts("");
		printf("%.2X ", rx[ret]);
	}
	puts("");
}
#endif
//opens a device to do read/write on. returns a descriptor.
int spiOpen(const char *spidevice)
{
	int filedescriptor = open(spidevice, O_RDWR);
	if (filedescriptor < 0)
		pabort("can't open device");
	return filedescriptor;
}
/* initializes spi in mode 0.0 with speed 500K,
 * and bit length 8.
 * with 0 delay.
 * */
void spi_init(int fd)
{
	spi_set_mode(fd, spi_mode);
	spi_set_word(fd, spi_bits);
	spi_set_speed(fd, spi_speed);
	spi_set_delay(0);
}
/*
 * prints detail on mode. speed and word size.
 * */
#ifdef DEBUG
void printSpiDetails()
{
	printf("spi mode: %d\n", spi_mode);
	printf("bits per word: %d\n", spi_bits);
	printf("max speed: %d Hz (%d KHz)\n", spi_speed, spi_speed/1000);
}
#endif
//closes device with descriptor fd.
int spiClose(int fd)
{
	//error checking?
	int ret = close(fd);
	return ret;
}
