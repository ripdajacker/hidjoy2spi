/*
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
 * author: Robert/Duality
 * git: github.com/duality4y
 *
 * Unused methods and constants removed to better fit the use case of
 * the hidjoy2spi project.
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

#define SPI_BUFFER_SIZE 20

extern uint8_t spi_mode;
extern uint8_t spi_bits;
extern uint32_t spi_speed;

void pabort(const char *s);

int transfer(int, uint8_t *, uint8_t *);

int spi_set_mode(int, uint8_t);

int spi_set_word(int, uint8_t);

int spi_set_speed(int, uint32_t);

void spi_set_delay(int);

void spi_init(int);

#endif

