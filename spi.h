/*
 * author: Robert/Duality
 * git: github.com/duality4y
 * 
 * source inspired by:
 * http://www.brianhensley.net/2012/07/getting-spi-working-on-raspberry-pi.html
 * 
 * also verry usefull little document:
 * http://lxr.free-electrons.com/source/include/uapi/linux/spi/spidev.h#L52
 * 
 * this source is for communicating over the spi. that the raspberrypi offers.
 * 
 * 
 * a rewrite to make it more like a "library"
 * for easier use.
 * although by now it is heavely modified.
 * 
 * spi.h
 *
 * Copyright (C) 2014 Duality
 *      Robert van der Tuuk <201292@live.nl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 * 
 * */

#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
//default spi buffer transmist/receive size.
#ifndef SPI_BUFFER_SIZE
	#define SPI_BUFFER_SIZE 100
#endif

//for debugging purposes.
//#define DEBUG

//static const char *spi_device = "/dev/spidev0.0";
extern uint8_t spi_mode;
extern uint8_t spi_bits;
extern uint32_t spi_speed;
extern uint16_t spi_delay;

void pabort(const char *s);
int transfer(int, uint8_t *, uint8_t *);
int spi_set_mode(int, int);
int spi_set_word(int, int);
int spi_set_speed(int, int);
void spi_set_delay(int);
int spiOpen(const char*);
int spiClose(int);
void spi_init(int);

#ifdef DEBUG
void loopbackTest(int);
void printSpiDetails();
#endif

#endif

